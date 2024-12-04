#include <edf/os/thread.hpp>

#include "esp_log.h"

#include "log_config.hpp"

extern "C" void app_main(void) {
  ESP_LOGI("app_main", "Ahoi Discoverer, your quest begins here!");

  static auto log_sink = log_config{};

  edf::os::heap_thread blink_thread({"test", edf::os::LowestTaskPriority, 1'024 * 5}, [] {
    ESP_LOGI("app_main thread", "Ahoi freeRTOS + EDF!");
    LOG_DEBUG("Ahoi Polaris Shared :)");
  });
  blink_thread.join();
}
