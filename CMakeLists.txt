cmake_minimum_required(VERSION 2.8.12)
project(Cycleshooter C CXX)


set(ASSETS_DIR "${PROJECT_SOURCE_DIR}/assets")
set(DEST_DIR "${PROJECT_BINARY_DIR}/dest")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(TESTS_DIR "${PROJECT_SOURCE_DIR}/tests")
set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")

set(LIBRARY_DIR "$ENV{HOME}/.lib")


option(BUILD_TESTS "Build tests." ON)
option(DEPENDENCIES_READY "Are the dependencies ready?" OFF)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${DEST_DIR}/bin")


add_definitions(-std=c++11)


add_subdirectory(${VENDOR_DIR})


if(DEPENDENCIES_READY)
    find_package(Bullet REQUIRED)

    if(NOT BULLET_FOUND)
        message(SEND_ERROR "Failed to find Bullet.")
    endif()

    include_directories(${BULLET_INCLUDE_DIRS})


    find_package(SFML REQUIRED EXACT 2.2 COMPONENTS audio)

    if(NOT SFML_FOUND)
        message(SEND_ERROR "Failed to find SFML.")
    endif()

    include_directories(${SFML_INCLUDE_DIR})


    find_package(OGRE REQUIRED EXACT 1.9)

    if(NOT OGRE_FOUND)
        message(SEND_ERROR "Failed to find OGRE. Is it installed? Is it the correct version (Ogre 1.9 Ghadamon)?")
    endif()

    include_directories(
        ${OGRE_INCLUDE_DIRS}
        ${OGRE_SAMPLES_INCLUDEPATH}
        ${OGRE_Terrain_INCLUDE_DIRS}
        ${OGRE_Overlay_INCLUDE_DIRS}
        )


    find_package(OIS REQUIRED)

    if(NOT OIS_FOUND)
        message(SEND_ERROR "Failed to find OIS.")
    endif()

    include_directories(${OIS_INCLUDE_DIRS})


    # Components that need linking (NB does not include header-only components like bind)
    set(OGRE_BOOST_COMPONENTS date_time thread)

    # Statically linking boost to a dynamic Ogre build doesn't work on Linux 64-bit
    set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})
    find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
    if (NOT Boost_FOUND)
        set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
        find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
    endif()
    find_package(Boost QUIET)

    include_directories(${Boost_INCLUDE_DIR})
    add_definitions(-DBOOST_ALL_NO_LIB)

    set(
        OGRE_LIBRARIES
        ${OGRE_LIBRARIES}
        ${Boost_LIBRARIES}
        ${OGRE_Terrain_LIBRARIES}
        ${OGRE_Overlay_LIBRARIES}
        )

    file(
        GLOB PROJECT_LIB_HEADERS
        "${SRC_DIR}/lib/*.h"
        "${SRC_DIR}/lib/*.hpp"
        )

    file(
        GLOB PROJECT_LIB_SOURCES
        "${SRC_DIR}/lib/*.cpp"
        )

    add_library(
        PROJECT_LIB OBJECT
        ${PROJECT_LIB_HEADERS}
        ${PROJECT_LIB_SOURCES}
        )


    file(
        GLOB PROJECT_HEADERS
        "${SRC_DIR}/include/*.h"
        "${SRC_DIR}/include/*.hpp"
        )

    file(
        GLOB PROJECT_SOURCES
        "${SRC_DIR}/src/*.cpp"
        )
        
    include_directories(
        "${SRC_DIR}/include"
        )

    add_executable(
        ${PROJECT_NAME}
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES}
        $<TARGET_OBJECTS:PROJECT_LIB>
        )

    target_link_libraries(
        ${PROJECT_NAME}
        ${BULLET_LIBRARIES}
        ${SFML_LIBRARIES}
        ${OGRE_LIBRARIES}
        ${OIS_LIBRARIES}
        )

    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target install
        )

    add_custom_target(
        run
        ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target install
        COMMAND ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        )


    file(
        GLOB_RECURSE IN_FILES
        "${ASSETS_DIR}/*.in"
        )

    # configure *.in -> *
    foreach(IN_FILE ${IN_FILES})
        string(REGEX REPLACE "\\.in$" "" OUT_FILE "${IN_FILE}")
        configure_file(
            ${IN_FILE}
            ${OUT_FILE}
            @ONLY
            )
        unset(OUT_FILE)
    endforeach()

    install(
        DIRECTORY ${ASSETS_DIR}/
        DESTINATION ${DEST_DIR}
        PATTERN "*.in"
        EXCLUDE
        )
else()
    message(WARNING "DEPENDENCIES_READY variable is set to OFF; project won't build.")
endif()
