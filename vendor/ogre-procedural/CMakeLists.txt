# Ogre Procedural

set(OgreProcedural_HOME ${LIBRARY_DIR} CACHE PATH "OgreProcedural library installation directory.")
find_package(OgreProcedural)

if(NOT OgreProcedural_FOUND)
    project(ogre_procedural_builder C CXX)

    include(ExternalProject)
    ExternalProject_Add(
        OgreProcedural
        DEPENDS ogre
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
        URL "https://bitbucket.org/transporter/ogre-procedural/get/ca6eb3363a53.zip"
        UPDATE_COMMAND ""
        CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${OgreProcedural_HOME} -DCMAKE_BUILD_TYPE:STRING=Release -DOgreProcedural_BUILD_DOCS:BOOL=FALSE -DOgreProcedural_BUILD_SAMPLES:BOOL=FALSE
        INSTALL_COMMAND ${CMAKE_COMMAND} --build . --target install
)
endif()

set(CMAKE_MODULE_PATH "${OgreProcedural_HOME}/lib/OgreProcedural/cmake;${CMAKE_MODULE_PATH}")
list(REMOVE_DUPLICATES CMAKE_MODULE_PATH)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH}" CACHE INTERNAL "" FORCE)
