list(APPEND CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_CXX_FLAGS_DEBUG
    "-g -O1 -fsanitize=undefined -fsanitize=address -fno-omit-frame-pointer -fno-optimize-sibling-calls"
    CACHE STRING "Flags used by the C++ compiler during coverage builds." FORCE)

# Get cmake helper functions (later via conan, right now via FetchContent)
include(FetchContent)
FetchContent_Declare(
  build_api
  GIT_REPOSITORY https://github.com/markuseggenbauer/build_api.git
  GIT_TAG main)

FetchContent_GetProperties(build_api)
if(NOT build_api_POPULATED)
  FetchContent_Populate(build_api)
endif()
list(APPEND CMAKE_MODULE_PATH ${build_api_SOURCE_DIR}/cmake/lib/src)
set(ME_CMAKE_SOURCE_DIR ${build_api_SOURCE_DIR}/cmake/lib/src)

# Conan package manager
if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
  message(
    STATUS
      "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
  file(
    DOWNLOAD
    "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
    "${CMAKE_BINARY_DIR}/conan.cmake")
endif()

include(${CMAKE_BINARY_DIR}/conan.cmake)

conan_cmake_run(
  CONANFILE
  conanfile.txt
  BASIC_SETUP
  CMAKE_TARGETS
  NO_OUTPUT_DIRS
  BUILD
  missing)

include(CTest)
include(me_build_api)
