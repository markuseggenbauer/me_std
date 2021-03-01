cmake_minimum_required(VERSION 3.16)

project(me_std)

include(setup.cmake)

find_package(GTest)

add_subdirectory(impl)

me_add_library(
    me_std
    CONTAINS pkg_me_std
)
