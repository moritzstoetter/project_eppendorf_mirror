#pragma once

#include <chrono>

#include "freertos/FreeRTOS.h"

namespace os {
using ticks = std::chrono::duration<uint32_t, std::ratio<1, 1'000 / portTICK_PERIOD_MS>>;

constexpr auto forever = ticks{portMAX_DELAY};
}    // namespace os_time
