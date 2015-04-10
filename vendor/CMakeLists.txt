get_filename_component(MODULE_PATH . ABSOLUTE)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${MODULE_PATH}")
unset(MODULE_PATH)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(googletest)

    if(DEPENDENCIES_READY)
        set(TEST_PROJECT_NAME "${PROJECT_NAME}Test")


        find_package(Threads REQUIRED)

        if(NOT Threads_FOUND)
            message(SEND_ERROR "Failed to find Threads.")
        endif()


        file(
            GLOB_RECURSE TEST_SRC_FILES
            "${TESTS_DIR}/*.cpp"
            )

        include_directories(
            ${GTEST_INCLUDE_DIRS}
            ${SRC_DIR}
            )

        add_executable(
            ${TEST_PROJECT_NAME}
            ${TEST_SRC_FILES}
            $<TARGET_OBJECTS:PROJECT_LIB>
            )

        add_dependencies(
            ${TEST_PROJECT_NAME}
            googletest
            )

        target_link_libraries(
            ${TEST_PROJECT_NAME}
            ${GTEST_BOTH_LIBRARIES}
            ${CMAKE_THREAD_LIBS_INIT}

            ${BULLET_LIBRARIES}
            ${SFML_LIBRARIES}
            ${OGRE_LIBRARIES}
            ${OIS_LIBRARIES}
            )

        add_custom_target(
            run_tests
            COMMAND ${TEST_PROJECT_NAME}
            WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
            )
    endif()
endif()

add_subdirectory(bullet)
add_subdirectory(ogre)
add_subdirectory(sfml)
