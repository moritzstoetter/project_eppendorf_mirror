macro(setup_conan)
    if (NOT EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
        execute_process(
                COMMAND /home/user/.local/bin/conan install ${CMAKE_SOURCE_DIR} -pr=esp32c3 -if=${CMAKE_BINARY_DIR} --build=missing
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        )
    endif ()

    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endmacro()