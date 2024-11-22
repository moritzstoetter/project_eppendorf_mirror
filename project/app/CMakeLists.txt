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
        app_main.cpp
)

target_include_directories(${APP}
        PRIVATE
        ${CMAKE_SOURCE_DIR}
)

#target_include_directories(metl
#        INTERFACE
#        $ENV{IDF_PATH}/components/freertos/FreeRTOS-Kernel-SMP/include/freertos
#)

target_link_libraries(${APP}
        PRIVATE
        #        metl

        idf::driver
        idf::freertos
)

target_compile_options(${APP}
        PRIVATE
        -fno-exceptions
)

build_executable(${APP})
