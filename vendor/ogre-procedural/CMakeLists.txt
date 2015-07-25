# Ogre Procedural

set(OgreProcedural_HOME ${LIBRARY_DIR} CACHE PATH "OgreProcedural library installation directory.")
find_package(OgreProcedural)

if(NOT OgreProcedural_FOUND)
    project(ogre_procedural_builder C CXX)

    include(ExternalProject)
    ExternalProject_Add(
        OgreProcedural
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
        HG_REPOSITORY "https://bitbucket.org/transporter/ogre-procedural"
        UPDATE_COMMAND ""
        CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${OgreProcedural_HOME} -DCMAKE_BUILD_TYPE:STRING=Release
        INSTALL_COMMAND ${CMAKE_COMMAND} --build . --target install
)
endif()
