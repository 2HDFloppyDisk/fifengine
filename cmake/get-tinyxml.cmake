#------------------------------------------------------------------------------
# External Project:     TinyXML v1
# Website:              https://sourceforge.net/projects/tinyxml 
# Downloads:            https://sourceforge.net/projects/tinyxml/files/tinyxml/                                 
#------------------------------------------------------------------------------

include(ExternalProject)

set(TINYXML_VERSION "2.6.2")

ExternalProject_Add(
  tinyxml
  PREFIX                ${DEPENDENCY_EXTRACT_DIR}
  DOWNLOAD_DIR          ${DEPENDENCY_DOWNLOAD_DIR}
  DOWNLOAD_NAME         tinyxml-v${TINYXML_VERSION}.zip
  URL                   https://sourceforge.net/projects/tinyxml/files/tinyxml/2.6.2/tinyxml_2_6_2.zip/download  
  URL_MD5               2a0aaf609c9e670ec9748cd01ed52dae
  PATCH_COMMAND         ${CMAKE_COMMAND} -E copy ${CMAKE_MODULE_PATH}/patches/tinyxml/CMakeLists.txt ${DEPENDENCY_EXTRACT_DIR}/src/tinyxml
  INSTALL_COMMAND 
    COMMAND             ${CMAKE_COMMAND} -E copy ${DEPENDENCY_EXTRACT_DIR}/src/tinyxml-build/Release/tinyxml.dll   ${DEPENDENCY_INSTALL_DIR}/bin/tinyxml.dll    
    COMMAND             ${CMAKE_COMMAND} -E copy ${DEPENDENCY_EXTRACT_DIR}/src/tinyxml-build/Release/tinyxml.lib   ${DEPENDENCY_INSTALL_DIR}/lib/tinyxml.lib    
    COMMAND             ${CMAKE_COMMAND} -E copy ${DEPENDENCY_EXTRACT_DIR}/src/tinyxml/tinystr.h"                  ${DEPENDENCY_INSTALL_DIR}/include/tinystr.h
    COMMAND             ${CMAKE_COMMAND} -E copy ${DEPENDENCY_EXTRACT_DIR}/src/tinyxml/tinyxml.h"                  ${DEPENDENCY_INSTALL_DIR}/include/tinyxml.h
  
)