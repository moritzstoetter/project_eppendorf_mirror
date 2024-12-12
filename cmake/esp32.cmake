macro(setup_build_env COMPONENTS)
    message(STATUS "Setting up build system")

    include($ENV{IDF_PATH}/tools/cmake/idf.cmake)

    set(valid_targets "esp32" "esp32s2" "esp32s3" "esp32c3" "esp32c2" "esp32c6" "esp32h2" "esp32p4")
    if (NOT DEFINED ENV{IDF_TARGET})
        message(FATAL_ERROR "Environment variable IDF_TARGET not set, valid choices are ${valid_targets}")
    elseif ($ENV{IDF_TARGET} IN_LIST valid_targets)
        idf_build_process($ENV{IDF_TARGET}
                COMPONENTS ${COMPONENTS}
                SDKCONFIG ${CMAKE_BINARY_DIR}/sdkconfig
                SDKCONFIG_DEFAULTS ${CMAKE_SOURCE_DIR}/sdkconfig.defaults
                BUILD_DIR ${CMAKE_BINARY_DIR}
        )
    else ()
        message(FATAL_ERROR "Unknown target: ${TARGET}")
    endif ()
endmacro()


macro(build_executable ELF)
    message(STATUS "Building executable")

    idf_build_executable(${ELF})

    add_custom_target(
            ${ELF}.bin
            COMMAND ${ESPTOOLPY} elf2image -o "${ELF}.bin" ${ELF}
            DEPENDS ${ELF}
    )
endmacro()

