# Bullet C++ Physics library

set(BULLET_ROOT ${LIBRARY_DIR} CACHE PATH "Bullet library installation directory.")
find_package(Bullet)

if(NOT BULLET_FOUND)
    project(bullet_builder C CXX)

    include(ExternalProject)
    ExternalProject_Add(
        bullet
        PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
        GIT_REPOSITORY "https://github.com/bulletphysics/bullet3"
        GIT_TAG 2.83
        UPDATE_COMMAND ""
        CMAKE_CACHE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=${BULLET_ROOT} -DCMAKE_BUILD_TYPE:STRING=Release -DBUILD_DEMOS:BOOL=OFF -DBUILD_SHARED_LIBS:BOOL=ON -DBUILD_MULTITHREADING:BOOL=OFF -DBUILD_EXTRAS:BOOL=ON -DINSTALL_LIBS:BOOL=ON -DINSTALL_EXTRA_LIBS:BOOL=ON
        INSTALL_COMMAND ${CMAKE_COMMAND} --build . --target install
        )
endif()

mark_as_advanced(BULLET_INCLUDE_DIR)
