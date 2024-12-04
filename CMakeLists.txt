cmake_minimum_required(VERSION 3.22)

project(esp-firmware C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

execute_process(
        COMMAND /home/user/.local/bin/conan install ${CMAKE_SOURCE_DIR} -pr=esp32c3 -if=${CMAKE_BINARY_DIR} --build=missing
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

## app
add_subdirectory(app)

add_custom_target(
        BUILD
        COMMAND bash -c \"./do.sh\"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        DEPENDS conanbuildinfo.cmake
)
