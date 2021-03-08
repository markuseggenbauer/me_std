cmake_minimum_required(VERSION 3.16)

project(me_std)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

find_program(CONAN_CMD conan)
execute_process(
    COMMAND "${CONAN_CMD}" install --generator cmake_find_package --update --build missing -if
            "${CMAKE_BINARY_DIR}" "." WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)

find_package(me_find_package)
include(me_find_package)

me_find_package(me_build)
me_find_package(
    GTest
    COMPONENTS
    gtest
    gtest_main
    gmock
    gmock_main
)

include(me_build)

add_subdirectory(impl)

me_add_library(me_std CONTAINS pkg_me_std)
