cmake_minimum_required(VERSION 3.16)

project(me_std)

list(
    APPEND
    CMAKE_MODULE_PATH
    ${CMAKE_BINARY_DIR}
)

find_package(build_api)
include(me_build_api)

me_find_package(
    GTest
    COMPONENTS
    gtest
    gtest_main
    gmock
    gmock_main
)

add_subdirectory(impl)

me_add_library(
    me_std
    CONTAINS pkg_me_std
)
