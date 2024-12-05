cmake_minimum_required(VERSION 3.22)

include(esp32)

setup_build_env(
        "driver"
        "freertos"
)

set(APP ${PROJECT_NAME}_app)

add_executable(${APP}
        app_main.cpp
        messaging/messenger.hpp
        messaging/decoder.hpp
)

target_sources(${APP}
        PRIVATE
        edf_stubs.cpp
)

target_include_directories(${APP}
        PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(${APP}
        PRIVATE
        idf::driver
        idf::freertos

        ${CONAN_LIBS}
)

build_executable(${APP})

