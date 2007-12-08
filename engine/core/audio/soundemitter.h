/***************************************************************************
 *   Copyright (C) 2005-2007 by the FIFE Team                              *
 *   fife-public@lists.sourceforge.net                                     *
 *   This file is part of FIFE.                                            *
 *                                                                         *
 *   FIFE is free software; you can redistribute it and/or modify          *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA              *
 ***************************************************************************/
 
#ifndef FIFE_SOUNDEMITTER_H_
#define FIFE_SOUNDEMITTER_H_

// Standard C++ library includes

// Platform specific includes

// 3rd party library includes

// FIFE includes
// These includes are split up in two parts, separated by one empty line
// First block: files included from the FIFE root src directory
// Second block: files included from the same folder
#include "util/singleton.h"
#include "vfs/raw/rawdata.h"
#include "util/time/timeevent.h"

#include "soundclip.h"

namespace FIFE {
	
	/** The class for playing audio files
	 */
	class SoundEmitter : private TimeEvent {
	public:
		SoundEmitter(unsigned int uid);
		
		~SoundEmitter();
		
		/** Returns the emitter-id
		 */
		unsigned int getID() {
			return m_emitterid;
		}
		
		/** Sets Positioning-Type
		 * Default is false
		 * 
		 * @param relative If set to true, the emitters position will be interpreted relative to the listener object
		 * 
		 */
		void setPositioning(bool relative) {
			alSourcei(m_source, AL_SOURCE_RELATIVE, relative ? AL_TRUE : AL_FALSE);
		}
		
		/** Loads an audio file
		 *
		 * @param filename The filename of the to be loaded audio file.
		 * @return true on success, false on failure
		 */
		bool load(const std::string &filename);
		
		/** Loads audio data from rawdata-ptr
		 *
		 * @param ptr RawData-pointer to the audio-data.
		 */
		bool load(RawData* ptr);
		
		/** Loads audio data using the specified decoder
		 * That's for matroska decoder-support
		 * (video module can use own decoder)
		 */
		bool loadFromDecoder(SoundDecoder* decoder);
		
		/** Reset the emitter, free all internal buffers
		 *  
		 * @param defaultall If set to true, emitter position, velocity, gain and type will be set to the default values
		 */
		void reset(bool defaultall = false);
		
		/** Sets the playing mode
		 */
		void setLooping(bool loop);
		
		/** Plays the associated audio file.
		 */
		void play();
		
		/** Stops playing the audio file and rewinds to the beginning
		 */
		void stop();
		
		/** Pauses playing the audio file
		 */
		void pause() {
			if (m_soundclip) {
				alSourcePause(m_source);
			}
		}
		
		/** Sets the gain of the emitter
		 *
		 * @param gain The gain value. 0=silence ... 1.0=normal loudness.
		 */
		void setGain(float gain) {
			alSourcef(m_source, AL_GAIN, gain);
		}
		
		/** Returns the gain of the emitter
		 *
		 * @return The gain value. 0=silence ... 1.0=normal loudness.
		 */
		float getGain() {
			float tmp;
			alGetSourcef(m_source, AL_GAIN, &tmp);
			return tmp;
		}
		
		/** Tests if the audio data is stereo data or mono.
		 * 
		 * @return Returns true if the audio data is stereo, false if mono.
		 */		
		bool isStereo() {
			if (m_soundclip) {
				return m_soundclip->getDecoder()->isStereo();
			}
			return false;
		}
		
		/** Returns the bit resolution
		 */
		short getBitResolution() {
			if (m_soundclip) {
				return m_soundclip->getDecoder()->getBitResolution();
			}
			return 0;
		}
		
		/** Returns the sample rate
		 */
		unsigned long getSampleRate() {
			if (m_soundclip) {
				return m_soundclip->getDecoder()->getSampleRate();
			}
			return 0;
		}
		
		/** Sets the cursor position in the audio file
		 */
		void setCursor(SoundPositionType type, float value);
		
		/** Returns the cursor position in the audio file
		 */
		float getCursor(SoundPositionType type);
		
		/** Sets the position of the SoundEmitter in the virtual audio space.
		 */
		void setPosition(float x, float y, float z) {
			alSource3f(m_source, AL_POSITION, x, y, z);
		}
		
		/** Sets the velocity of the SoundEmitter in the virtual audio space.
		 */
		void setVelocity(float x, float y, float z) {
			alSource3f(m_source, AL_VELOCITY, x, y, z);
		}
		
	private:
		/** Implementation of the pure virtual function from TimeEvent to update streaming
		 */
		virtual void updateEvent(unsigned long time);
		
		ALuint							m_source;			// The openAL-source
		SoundClip*					m_soundclip;	// the attached soundclip
		unsigned int				m_soundclipid;// id of the attached soundclip
		unsigned int				m_streamid;		// the id of the stream
		unsigned int				m_emitterid;	// the emitter-id
		bool								m_loop;				// loop?
	};
}

#endif
