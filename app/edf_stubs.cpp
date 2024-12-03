#include <edf/fail/failure_handler.hpp>
#include <edf/hw/mcu.hpp>

#include "esp_system.h"

namespace edf::hw::mcu {

auto get_isr_number() noexcept -> uint32_t {
    // Unimplemented.
    return 0;
}

auto reset() noexcept -> void {
    esp_restart();
}

auto get_hardware_reset_reason() noexcept -> std::optional <std::string_view> {
    static std::string str;

    switch (esp_reset_reason()) {
        case ESP_RST_UNKNOWN:       str = "ESP_RST_UNKNOWN"; break;
        case ESP_RST_POWERON:       str = "ESP_RST_POWERON"; break;
        case ESP_RST_EXT:           str = "ESP_RST_EXT"; break;
        case ESP_RST_SW:            str = "ESP_RST_SW"; break;
        case ESP_RST_PANIC:         str = "ESP_RST_PANIC"; break;
        case ESP_RST_INT_WDT:       str = "ESP_RST_INT_WDT"; break;
        case ESP_RST_TASK_WDT:      str = "ESP_RST_TASK_WDT"; break;
        case ESP_RST_WDT:           str = "ESP_RST_WDT"; break;
        case ESP_RST_DEEPSLEEP:     str = "ESP_RST_DEEPSLEEP"; break;
        case ESP_RST_BROWNOUT:      str = "ESP_RST_BROWNOUT"; break;
        case ESP_RST_SDIO:          str = "ESP_RST_SDIO"; break;
        case ESP_RST_USB:           str = "ESP_RST_USB"; break;
        case ESP_RST_JTAG:          str = "ESP_RST_JTAG"; break;
        case ESP_RST_EFUSE:         str = "ESP_RST_EFUSE"; break;
        case ESP_RST_PWR_GLITCH:    str = "ESP_RST_PWR_GLITCH"; break;
        case ESP_RST_CPU_LOCKUP:    str = "ESP_RST_CPU_LOCKUP"; break;
        default:                    return std::nullopt;
    }

    return std::string_view{str};
}

}

namespace edf::fail {

auto get_failure_handler() -> failure_handler & {
    std::terminate();
}

}
