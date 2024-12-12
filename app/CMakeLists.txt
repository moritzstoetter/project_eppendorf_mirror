include(esp32)

setup_build_env(
        "esp_wifi"
        "nvs_flash"
        "wpa_supplicant"
        "driver"
        "freertos"
)

set(APP ${PROJECT_NAME}_app)

add_executable(${APP}
        app_main.cpp
        edf_stubs.cpp
        ../util/string.hpp
)

target_include_directories(${APP}
        PRIVATE
        ${CMAKE_SOURCE_DIR}
)

add_subdirectory(modules)

target_link_libraries(${APP}
        PRIVATE
        idf::driver
        idf::freertos
        idf::esp_wifi
        idf::nvs_flash
        idf::wpa_supplicant

        ${CONAN_LIBS}
)

build_executable(${APP})
