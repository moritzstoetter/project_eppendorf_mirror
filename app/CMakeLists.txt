cmake_minimum_required(VERSION 3.22)

include(esp32)

setup_build_env(
        "driver"
        "freertos"
)

set(APP ${PROJECT_NAME}_app)

add_executable(${APP}
        app_main.cpp
)

target_sources(${APP}
        PRIVATE
)

target_include_directories(${APP}
        PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${APP}
        PRIVATE
        idf::driver
        idf::freertos
)

target_compile_options(${APP}
        PRIVATE
        -fno-exceptions
)

build_executable(${APP})
