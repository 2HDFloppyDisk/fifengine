# MapEditor is a plugin for Fifedit. It allows for selection and visual editing of maps.
# MapEditor must be pumped (see pump).

import math

import fife
import pychan
import pychan.widgets as widgets
from viewer import Viewer

class Selection():
	"""
	Selection displays a list of options for the user to select from. The result is passed to onSelection.
	list - the list to select from
	onSelection - the function to call when a selection is made. Accepts one argument: an element of the list.
	"""
	def __init__(self, list, onSelection):
		self.list = list
		self._callback = onSelection

		self._widget = pychan.loadXML('content/gui/selection.xml')

		self._widget.mapEvents({
			'okButton'     : self._selected,
			'cancelButton' : self._widget.hide
		})

		self._widget.distributeInitialData({
			'optionDrop' : list
		})
		self._widget.show()

	def _selected(self):
		selection = self._widget.collectData(['optionDrop'])['optionDrop']
		if selection < 0: return
		self._callback(self.list[selection])
		self._widget.hide()

class MapEditor(fife.IMouseListener, fife.IKeyListener):
	def __init__(self, engine):
		self.engine = engine
		self.eventmanager = self.engine.getEventManager()

		fife.IMouseListener.__init__(self)
		self.eventmanager.addMouseListener(self)
		fife.IKeyListener.__init__(self)
		self.eventmanager.addKeyListener(self)

		# Fifedit plugin data
		self.menu_items = { 'Edit' : self._selectMap }

		# MapEditor needs a viewer so the user can see things.
		self.viewer = Viewer(self.engine)

		self.mapSelect = None
		self.datSelect = None

		self.mapEdit = None
		self.camera = None
		self.elevation = None
		self.layer = None
		self.selection = None

		self.dataset = None
		self.object = None

	# gui for selecting a map
	def _selectMap(self):
		Selection([map.Id() for map in self.engine.getModel().getMaps()], self.editMap)

	def editMap(self, mapid):
		self.viewer.viewMap(mapid)
		self.map = self.engine.getModel().getMaps('id', mapid)[0]
		self.camera = self.viewer.camera
		self.elevation = self.camera.getLocation().getElevation()
		self.layer = self.camera.getLocation().getLayer()

		if not self.mapEdit:
			self.mapEdit = pychan.loadXML('content/gui/mapeditor.xml')
			self.mapEdit.mapEvents({
				'datButton'   : self._selectDataset,
				'closeButton' : self.quit
			})

		metafields = self.mapEdit.findChild(name='Metadata Properties')
		for metafield in self.map.listFields():
			hbox = widgets.HBox()
			metafields.add(hbox)

			label = widgets.Label(text=metafield)
			hbox.add(label)
			field = widgets.TextField(text=self.map.get(metafield))
			hbox.add(field)

		self.mapEdit.show()

	def _selectDataset(self):
		Selection([dat.Id() for dat in self.map.getDatasets()], self._viewDataset)

	def _viewDataset(self, datid):
		self.dataset = self.engine.getModel().getMetaModel().getDatasets('id',  datid)[0]
		Selection([obj.Id() for obj in self.dataset.getObjects()], self.editWith)

	def editWith(self, object_id):
		if self.dataset:
			self.object = self.dataset.getObjects('id', object_id)[0]

	def pump(self):
		self.viewer.pump()

	def quit(self):
#   Sleek: why doesn't this work?
#		self.viewer.deactivate() 
		self.camera = None
		self.elevation = None
		self.layer = None
		self.selection = None

		self.mapEdit.hide()

	def mousePressed(self, evt):
		if self.camera:
			# TODO: make Sleek fix this ugly mess
			tmp = fife.ScreenPoint(evt.getX(), evt.getY())
			dy = -(tmp.y - self.camera.toScreenCoordinates(self.camera.getLocation().getElevationCoordinates()).y)
			tmp.z = (int)(math.tan(self.camera.getTilt()* (math.pi / 180.0)) * dy)
			self.selection = self.camera.toElevationCoordinates(tmp)
			self.selection.z = 0
			self.selection = self.layer.getCellGrid().toLayerCoordinates(self.selection)

	def mouseReleased(self, evt):
		pass
	def mouseEntered(self, evt):
		pass
	def mouseExited(self, evt):
		pass
	def mouseClicked(self, evt):
		pass
	def mouseWheelMovedUp(self, evt):
		pass
	def mouseWheelMovedDown(self, evt):
		pass
	def mouseMoved(self, evt):
		pass
	def mouseDragged(self, evt):
		pass

	def keyPressed(self, evt):
		keyval = evt.getKey().getValue()
		keystr = evt.getKey().getAsString().lower()
		if keystr == 'm':
			pass # TODO: make an instance
			if self.selection and self.object:
				inst = self.layer.createInstance(self.object, self.selection)
				fife.InstanceVisual.create(inst)

		elif keystr == 'x':
			if self.selection:
				for inst in self.layer.getInstances('loc', str(self.selection.x) + ',' + str(self.selection.y)):
					self.layer.deleteInstance(inst)

	def keyReleased(self, evt):
		pass
