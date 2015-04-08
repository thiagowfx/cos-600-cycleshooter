# Google Test C++ library
# Forked from: https://github.com/snikulov/google-test-examples
#
# EXPORTS:
#
# - GTEST_INCLUDE_DIRS
# - GTEST_LIBS_DIR
#
# - GTEST_BOTH_LIBRARIES - both libgtest & libgtest-main
# - GTEST_LIBRARIES - libgtest
# - GTEST_MAIN_LIBRARIES - libgtest-main

project(googletest_builder C CXX)

include(ExternalProject)
ExternalProject_Add(
    googletest
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    SVN_REPOSITORY "http://googletest.googlecode.com/svn/trunk"
    UPDATE_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Get_Property(googletest SOURCE_DIR)
set(GTEST_INCLUDE_DIRS ${SOURCE_DIR}/include CACHE INTERNAL "googletest include directories")

ExternalProject_Get_Property(googletest BINARY_DIR)
set(GTEST_LIBS_DIR ${BINARY_DIR} CACHE INTERNAL "googletest library directory")

set(GTEST_LIBRARIES ${BINARY_DIR}/libgtest.a CACHE INTERNAL "libgtest")
set(GTEST_MAIN_LIBRARIES ${BINARY_DIR}/libgtest_main.a CACHE INTERNAL "libgtest-main")
set(GTEST_BOTH_LIBRARIES ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} CACHE INTERNAL "both libgtest & libgtest-main")
