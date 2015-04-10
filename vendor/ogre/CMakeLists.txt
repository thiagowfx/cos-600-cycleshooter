# Ogre C++ Graphics Library

set(OGRE_HOME ${LIBRARY_DIR} CACHE PATH "Ogre library installation directory.")
find_package(OGRE)

if(NOT OGRE_FOUND)
    project(ogre_builder C CXX)

    include(ExternalProject)
    ExternalProject_Add(
        ogre
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
        URL "https://bitbucket.org/sinbad/ogre/get/v1-9-0.zip"
        UPDATE_COMMAND ""
        CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${OGRE_HOME} -DCMAKE_BUILD_TYPE:STRING=Release -DOGRE_INSTALL_SAMPLES:BOOL=FALSE -DOGRE_INSTALL_DOCS:BOOL=FALSE -DOGRE_INSTALL_SAMPLES_SOURCE:BOOL=FALSE
        INSTALL_COMMAND ${CMAKE_COMMAND} --build . --target install
)
endif()

set(CMAKE_MODULE_PATH "${OGRE_HOME}/lib/OGRE/cmake;${CMAKE_MODULE_PATH}")
list(REMOVE_DUPLICATES CMAKE_MODULE_PATH)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" CACHE INTERNAL "" FORCE)

set(OGRE_SAMPLES_INCLUDEPATH "${OGRE_HOME}/share/OGRE/samples/Common/include/" CACHE INTERNAL "" FORCE)
