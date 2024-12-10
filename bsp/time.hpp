#pragma once

#include <chrono>

#include "freertos/FreeRTOS.h"

namespace os {
using ticks = std::chrono::duration<uint32_t, std::ratio<1, 1'000 / portTICK_PERIOD_MS>>;

constexpr auto forever = std::chrono::milliseconds::max();

/**
 * Converts milliseconds to ticks.
 * The milliseconds will be clamped: 0 <= duration <= portMAX_DELAY
 *
 * @param duration - the duration
 * @return the ticks
 */
[[nodiscard]] constexpr TickType_t to_ticks(const std::chrono::milliseconds& duration) noexcept {
  return pdMS_TO_TICKS(std::clamp<std::chrono::milliseconds::rep>(duration.count(), 0, portMAX_DELAY));
}

/**
 * Converts ticks to milliseconds.
 * The milliseconds will be clamped: 0 <= duration <= portMAX_DELAY
 *
 * @param duration - the duration
 * @return the milliseconds
 */
[[nodiscard]] constexpr std::chrono::milliseconds to_ms(TickType_t ticks) noexcept {
  return std::chrono::milliseconds{pdTICKS_TO_MS(ticks)};
}

}    // namespace os_time
