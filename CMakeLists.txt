cmake_minimum_required(VERSION 3.16)

project(me_std)

include(setup.cmake)

add_subdirectory(impl)
add_subdirectory(test)

me_add_library(
    me_std
    CONTAINS pkg_me_std
)
