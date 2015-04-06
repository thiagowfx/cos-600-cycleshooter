if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(googletest)

    set(TEST_PROJECT_NAME "${PROJECT_NAME}_TESTS")
    file(GLOB_RECURSE TEST_SRC_FILES "${TESTS_DIR}/*.cpp")

    include_directories(${GTEST_INCLUDE_DIRS})

    add_executable(
        ${TEST_PROJECT_NAME}
        ${TEST_SRC_FILES}
        )

    add_dependencies(
        ${TEST_PROJECT_NAME}
        googletest
        )

    target_link_libraries(
        ${TEST_PROJECT_NAME}
        ${GTEST_BOTH_LIBRARIES}
        pthread
        )

    add_custom_target(
        run_tests
        COMMAND "${TEST_PROJECT_NAME}"
        WORKING_DIRECTORY "${DEST_DIR}/bin"
        )
endif()

get_filename_component(MODULE_PATH . ABSOLUTE)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${MODULE_PATH}")
unset(MODULE_PATH)

add_subdirectory(bullet)
add_subdirectory(ogre)
