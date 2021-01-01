cmake_minimum_required(VERSION 3.16)

project(me_std)

include(dependencies.cmake)
include(CTest)

add_subdirectory(impl)
add_subdirectory(test)
