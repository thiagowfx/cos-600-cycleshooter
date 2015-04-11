# SFML C++ Multimedia Library

set(SFML_ROOT ${LIBRARY_DIR} CACHE PATH "SFML library installation directory.")
find_package(SFML EXACT 2.2 COMPONENTS audio)

if(NOT SFML_FOUND)
    project(sfml_builder C CXX)

    include(ExternalProject)
    ExternalProject_Add(
        sfml
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
        GIT_REPOSITORY "https://github.com/LaurentGomila/SFML"
        GIT_TAG 2.2
        UPDATE_COMMAND ""
        CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${SFML_ROOT} -DCMAKE_BUILD_TYPE:STRING=Release -DSFML_BUILD_EXAMPLES:BOOL=OFF -DSFML_BUILD_DOC:BOOL=OFF -DSFML_INSTALL_PKGCONFIG_FILES:BOOL=OFF
        INSTALL_COMMAND ${CMAKE_COMMAND} --build . --target install
        )
endif()
