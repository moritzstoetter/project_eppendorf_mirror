cmake_minimum_required(VERSION 3.22)

project(esp-firmware C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(conan)
setup_conan()

## app
add_subdirectory(app)
