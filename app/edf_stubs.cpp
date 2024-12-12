/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
 *  @date 2024-12-10 (created)
 *  @author Moritz Stötter (moritz@modernembedded.tech)
 *  @copyright (c) Eppendorf SE 2024 - Polaris Project
 *  @brief EDF stubs
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <edf/fail/failure_handler.hpp>
#include <edf/hw/mcu.hpp>

#include <optional>
#include <string_view>

#include "esp_system.h"

namespace edf::hw::mcu {

uint32_t get_isr_number() noexcept {
  // Unimplemented.
  return 0;
}

void reset() noexcept {
  esp_restart();
}

std::optional<std::string_view> get_hardware_reset_reason() noexcept {
  switch (esp_reset_reason()) {
  case ESP_RST_UNKNOWN:
    return {"ESP_RST_UNKNOWN"};
  case ESP_RST_POWERON:
    return {"ESP_RST_POWERON"};
  case ESP_RST_EXT:
    return {"ESP_RST_EXT"};
  case ESP_RST_SW:
    return {"ESP_RST_SW"};
  case ESP_RST_PANIC:
    return {"ESP_RST_PANIC"};
  case ESP_RST_INT_WDT:
    return {"ESP_RST_INT_WDT"};
  case ESP_RST_TASK_WDT:
    return {"ESP_RST_TASK_WDT"};
  case ESP_RST_WDT:
    return {"ESP_RST_WDT"};
  case ESP_RST_DEEPSLEEP:
    return {"ESP_RST_DEEPSLEEP"};
  case ESP_RST_BROWNOUT:
    return {"ESP_RST_BROWNOUT"};
  case ESP_RST_SDIO:
    return {"ESP_RST_SDIO"};
  case ESP_RST_USB:
    return {"ESP_RST_USB"};
  case ESP_RST_JTAG:
    return {"ESP_RST_JTAG"};
  case ESP_RST_EFUSE:
    return {"ESP_RST_EFUSE"};
  case ESP_RST_PWR_GLITCH:
    return {"ESP_RST_PWR_GLITCH"};
  case ESP_RST_CPU_LOCKUP:
    return {"ESP_RST_CPU_LOCKUP"};
  default:
    return std::nullopt;
  }
}

}    // namespace edf::hw::mcu

namespace edf::fail {

failure_handler& get_failure_handler() {
  std::terminate();
}

}    // namespace edf::fail
