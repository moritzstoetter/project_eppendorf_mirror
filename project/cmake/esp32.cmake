macro(setup_build_env COMPONENTS)
    message(STATUS "Setting up build system")

    include($ENV{IDF_PATH}/tools/cmake/idf.cmake)

    set(valid_targets "esp32" "esp32s2" "esp32s3" "esp32c3" "esp32c2" "esp32c6" "esp32h2" "esp32p4")
    if ($CACHE{TARGET} IN_LIST valid_targets)
        idf_build_process($CACHE{TARGET}
                COMPONENTS ${COMPONENTS}
                SDKCONFIG ${CMAKE_SOURCE_DIR}/sdkconfig
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

