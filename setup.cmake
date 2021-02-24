find_program(
    CONAN_CMD
    conan
)

if(CONAN_CMD)
    execute_process(
        COMMAND "${CONAN_CMD}" "install" "--build" "missing" "-if" "${CMAKE_BINARY_DIR}" "."
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )
else()
    message(FATAL_ERROR "Conan is required.")
endif()

list(
    APPEND
    CMAKE_MODULE_PATH
    ${CMAKE_BINARY_DIR}
)
list(
    APPEND
    CMAKE_PREFIX_PATH
    ${CMAKE_BINARY_DIR}
)
find_package(build_api)
include(CTest)
