#pragma once

#include <cstdint>

#include "esp_wifi.h"

#include "app/log_config.hpp"
#include "app/messaging/dispatch.hpp"

namespace wifi {

namespace requests {
struct start_service {};
struct stop_service {};
struct start_scan {};
}    // namespace requests

// clang-format off
using request_t =
  std::variant<
    requests::start_service,
    requests::stop_service,
    requests::start_scan
>;
// clang-format on

struct mod {
  enum struct state { uninitialized, off, on };

  void init() {
    if (state_ != state::uninitialized) {
      return;
    }

    ESP_ERROR_CHECK(esp_netif_init());
    esp_netif_inherent_config_t esp_netif_config = ESP_NETIF_INHERENT_DEFAULT_WIFI_STA();
    esp_netif_config.if_desc = "netif";
    esp_netif_config.route_prio = 128;
    assert(esp_netif_create_wifi(WIFI_IF_STA, &esp_netif_config));

    ::wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    ESP_ERROR_CHECK(esp_wifi_set_default_wifi_sta_handlers());
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, nullptr));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &ip_event_handler, nullptr));

    state_ = state::off;
  }

  void start() {
    init();
    ESP_ERROR_CHECK(esp_wifi_start());
    state_ = state::on;
  }

  void stop() {
    ESP_ERROR_CHECK(esp_wifi_stop());
    state_ = state::off;
  }

  private:
  state state_ = state::uninitialized;

  static auto ip_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) -> void;
  static auto wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) -> void;
};

constexpr auto make_handler(mod& wifi) {
  using namespace requests;

  return msg::handler{
    msg::callback<start_service>(msg::always_match,
                                 [&](auto&&) mutable {
                                   LOG_DEBUG("[WIFI] start service");
                                   wifi.start();
                                 }),
    msg::callback<stop_service>(msg::always_match,
                                [&](auto&&) mutable {
                                  LOG_DEBUG("[WIFI] stop service");
                                  wifi.stop();
                                }),
    msg::callback<start_scan>(msg::always_match,
                              [](auto&&) {
                                LOG_DEBUG("[WIFI] start scan");
                                constexpr auto conf = ::wifi_scan_config_t{};
                                constexpr auto block = false;

                                LOG_DEBUG("[WIFI] start scan");
                                esp_wifi_scan_start(&conf, block);
                              }),

  };
}
}    // namespace wifi
