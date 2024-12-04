#include <edf/fail/failure_handler.hpp>
#include <edf/hw/mcu.hpp>

#include <optional>
#include <string>
#include <string_view>

#include "esp_system.h"

namespace edf::hw::mcu {

auto get_isr_number() noexcept -> uint32_t {
    // Unimplemented.
    return 0;
}

auto reset() noexcept -> void {
    esp_restart();
}

auto get_hardware_reset_reason() noexcept -> std::optional<std::string_view> {
    switch (esp_reset_reason()) {
    case ESP_RST_UNKNOWN:    return {"ESP_RST_UNKNOWN"};
    case ESP_RST_POWERON:    return {"ESP_RST_POWERON"};
    case ESP_RST_EXT:        return {"ESP_RST_EXT"};
    case ESP_RST_SW:         return {"ESP_RST_SW"};
    case ESP_RST_PANIC:      return {"ESP_RST_PANIC"};
    case ESP_RST_INT_WDT:    return {"ESP_RST_INT_WDT"};
    case ESP_RST_TASK_WDT:   return {"ESP_RST_TASK_WDT"};
    case ESP_RST_WDT:        return {"ESP_RST_WDT"};
    case ESP_RST_DEEPSLEEP:  return {"ESP_RST_DEEPSLEEP"};
    case ESP_RST_BROWNOUT:   return {"ESP_RST_BROWNOUT"};
    case ESP_RST_SDIO:       return {"ESP_RST_SDIO"};
    case ESP_RST_USB:        return {"ESP_RST_USB"};
    case ESP_RST_JTAG:       return {"ESP_RST_JTAG"};
    case ESP_RST_EFUSE:      return {"ESP_RST_EFUSE"};
    case ESP_RST_PWR_GLITCH: return {"ESP_RST_PWR_GLITCH"};
    case ESP_RST_CPU_LOCKUP: return {"ESP_RST_CPU_LOCKUP"};
    default:                 return std::nullopt;
    }
}

} // namespace edf::hw::mcu

namespace edf::fail {

auto get_failure_handler() -> failure_handler & {
    std::terminate();
}

} // namespace edf::fail
