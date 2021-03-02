find_program(CONAN_CMD conan)

if(CONAN_CMD AND EXISTS "${CMAKE_CURRENT_LIST_DIR}/package_bootstrap_conanfile.txt")

    if(NOT ME_PACKAGE_FOUND)

        list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")
        list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

        execute_process(
            COMMAND "${CONAN_CMD}" install --build missing -if "${CMAKE_BINARY_DIR}"
                    "${CMAKE_CURRENT_LIST_DIR}/package_bootstrap_conanfile.txt"
            WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        )

        find_package(me_package)
        include(me_find_package)

    endif()

    execute_process(
        COMMAND "${CONAN_CMD}" install --build missing -if "${CMAKE_BINARY_DIR}" "."
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
    )

else()

    macro(me_find_package)
        find_package(${ARGN})
    endmacro()

endif()
