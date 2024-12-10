#include "app/log_config.hpp"
#include "app/messaging/messenger.hpp"

extern "C" [[noreturn]] void app_main() {
  static auto log_sink = esp_uart_log_sink{};
  static auto messenger = messenger_t{};

  while (true) {
    if (auto msg = messenger.receive()) {
      LOG_DEBUG("Message received");
    }
  }
}
