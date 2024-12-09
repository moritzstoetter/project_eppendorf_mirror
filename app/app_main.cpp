#include "app/log_config.hpp"
#include "app/messaging/messenger.hpp"
#include "app/modules/modules.hpp"

#include "nvs_flash.h"

extern "C" [[noreturn]] void app_main() {
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES or ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  static auto log_sink = esp_uart_log_sink{};

  constinit static struct {
    sys::mod sys;
    wifi::mod wifi;
  } modules;

  // clang-format off
  constexpr static auto msg_handler =
    msg::handler{
      sys::make_handler(modules.sys),
      wifi::make_handler(modules.wifi),
    };
  // clang-format on

  static auto messenger = messenger_t<request_t>{};

  while (true) {
    if (auto msg = messenger.receive()) {
      auto const dispatched = std::visit([&](auto &&m) { return msg_handler(m); }, *msg);
      if (not dispatched) {
        LOG_WARNING("Received unhandled message");
      }
    }
  }
}
