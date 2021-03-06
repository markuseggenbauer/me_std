cmake_minimum_required(VERSION 3.16)

project(me_std)

include(${CMAKE_CURRENT_LIST_DIR}/bootstrap.cmake)

me_find_package(me_build)
include(me_build)

me_find_package(
    GTest
    COMPONENTS
    gtest
    gtest_main
    gmock
    gmock_main
)

add_subdirectory(impl)

me_add_library(me_std CONTAINS pkg_me_std)
