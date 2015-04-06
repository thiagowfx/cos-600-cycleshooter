cmake_minimum_required(VERSION 2.8.12)
project(OgreBulletApp C CXX)


set(ASSETS_DIR "${PROJECT_SOURCE_DIR}/assets")
set(DEST_DIR "${PROJECT_SOURCE_DIR}/dest")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(TESTS_DIR "${PROJECT_SOURCE_DIR}/tests")
set(VENDOR_DIR "${PROJECT_SOURCE_DIR}/vendor")


option(BUILD_TESTS "Build tests." ON)
option(DEPENDENCIES_READY "Are the dependencies ready?" OFF)


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${DEST_DIR}/bin")


add_subdirectory(${VENDOR_DIR})


if(DEPENDENCIES_READY)
    find_package(Bullet REQUIRED)

    if(NOT BULLET_FOUND)
        message(SEND_ERROR "Failed to find Bullet.")
    endif()

    include_directories(${BULLET_INCLUDE_DIRS})


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


    # Statically linking boost to a dynamic Ogre build doesn't work on Linux 64-bit
    set(Boost_USE_STATIC_LIBS ${OGRE_STATIC})

    # Components that need linking (NB does not include header-only components like bind)
    set(OGRE_BOOST_COMPONENTS thread date_time)

    find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
    if (NOT Boost_FOUND)
        # Try again with the other type of libs
        set(Boost_USE_STATIC_LIBS NOT ${Boost_USE_STATIC_LIBS})
        find_package(Boost COMPONENTS ${OGRE_BOOST_COMPONENTS} QUIET)
    endif()
    find_package(Boost QUIET)

    include_directories(${Boost_INCLUDE_DIR})
    add_definitions(-DBOOST_ALL_NO_LIB)

    # Set up referencing of Boost
    set(
        OGRE_LIBRARIES
        ${OGRE_LIBRARIES}
        ${Boost_LIBRARIES}
        ${OGRE_Terrain_LIBRARIES}
        ${OGRE_Overlay_LIBRARIES}
        )


    file(
        GLOB_RECURSE
        PROJECT_HEADERS
        "${SRC_DIR}/*.h"
        "${SRC_DIR}/*.hpp"
        )

    file(
        GLOB_RECURSE
        PROJECT_SOURCES
        "${SRC_DIR}/*.cpp"
        )


    add_executable(
        ${PROJECT_NAME}
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES}
        )

    target_link_libraries(
        ${PROJECT_NAME}
        ${OGRE_LIBRARIES}
        ${OIS_LIBRARIES}
        ${BULLET_LIBRARIES}
        )

    add_custom_target(
        run
        ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target install
        COMMAND "${PROJECT_NAME}"
        WORKING_DIRECTORY "${DEST_DIR}/bin"
        )


    file(
        GLOB_RECURSE
        IN_FILES
        "${ASSETS_DIR}/*.in"
        )

    # configure file.in -> file
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
        DIRECTORY
        "${ASSETS_DIR}/"
        DESTINATION
        "${DEST_DIR}"
        PATTERN
        "*.in"
        EXCLUDE
        )
else()
    message(WARNING "DEPENDENCIES_READY is set to OFF; project won't build")
endif()
