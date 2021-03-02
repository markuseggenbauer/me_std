cmake_minimum_required(VERSION 3.1)
project(
    PackageTest
    CXX
)

find_package(build_api)
include(me_build_api)

me_find_package(me_std)

me_add_implementation_package(example)
me_add_packagetest(
    example
    SOURCES example.cpp
    SOURCE_DEPENDS me_std::me_std
)
