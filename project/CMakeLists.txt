cmake_minimum_required(VERSION 3.22)

project(esp-firmware C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(METL_LOG_CONFIG_DIR ${CMAKE_SOURCE_DIR}/app)

# dependencies
#add_subdirectory(metl)

## app
add_subdirectory(app)



