include(me_find_package OPTIONAL RESULT_VARIABLE ME_FIND_PACKAGE_FILE)
if(ME_FIND_PACKAGE_FILE STREQUAL "NOTFOUND")
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

    find_program(CONAN_CMD conan)
    execute_process(
        COMMAND "${CONAN_CMD}" install --build missing -if "${CMAKE_BINARY_DIR}" "."
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )

    find_package(me_package)
    include(me_find_package)
    me_package_init()
endif()
