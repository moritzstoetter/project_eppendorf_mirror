/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  @date 2024-12-10 (created)
 *  @author Moritz Stötter (moritz@modernembedded.tech)
 *  @copyright (c) Eppendorf SE 2024 - Polaris Project
 *  @brief Wifi service class.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
struct connect_to_ssid {
  std::string_view ssid;
  std::string_view pass;
};
struct set_hostname {
  std::string_view hostname;
};
}    // namespace requests

// clang-format off
using request_t =
  std::variant<
    requests::start_service,
    requests::stop_service,
    requests::start_scan,
    requests::connect_to_ssid,
    requests::set_hostname
>;
// clang-format on

struct mod {
  enum struct state { uninitialized, off, on, connected, got_ip };

  void start() {
    if (state_ != state::uninitialized) {
      deinit();
    }
    init();

    ESP_ERROR_CHECK(esp_event_handler_instance_register(
      WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, this, &wifi_event_handler_instance_));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(
      IP_EVENT, ESP_EVENT_ANY_ID, &ip_event_handler, this, &ip_event_handler_instance_));

    ESP_ERROR_CHECK(esp_wifi_start());
    state_ = state::on;
  }

  void stop() {
    ESP_ERROR_CHECK(esp_wifi_stop());
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, ESP_EVENT_ANY_ID, &ip_event_handler_instance_));
    ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler_instance_));
    state_ = state::off;
  }

  void set_hostname(std::string_view hostname) const {
    std::array<char, 32> buf{};
    const auto out = std::copy_n(hostname.begin(), std::clamp(hostname.size(), 0UZ, 31UZ), buf.begin());
    *out = '\0';
    ESP_ERROR_CHECK(esp_netif_set_hostname(netif_, buf.data()));
  }

  void connect(std::string_view ssid, std::string_view pass) const {
    const auto copy_null_terminate =
      []<std::ranges::input_range In, std::ranges::output_range<std::ranges::range_value_t<In>> Out>(In&& in,
                                                                                                     Out&& out) {
        auto [_, out_it] =
          std::ranges::copy_n(std::begin(in), std::clamp(std::size(in), 0UZ, std::size(out) - 1), std::begin(out));
        *out_it = '\0';
      };

    auto conf = ::wifi_config_t{};
    copy_null_terminate(ssid, conf.sta.ssid);
    copy_null_terminate(pass, conf.sta.password);

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &conf));
    ESP_ERROR_CHECK(esp_wifi_connect());
  }

  private:
  state state_ = state::uninitialized;
  esp_netif_t* netif_ = nullptr;
  esp_event_handler_instance_t ip_event_handler_instance_ = nullptr;
  esp_event_handler_instance_t wifi_event_handler_instance_ = nullptr;

  void init() {
    ESP_ERROR_CHECK(esp_netif_init());
    netif_ = esp_netif_create_default_wifi_sta();
    assert(netif_);

    const ::wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

    state_ = state::off;
  }

  void deinit() {
    stop();

    ESP_ERROR_CHECK(esp_wifi_deinit());
    ESP_ERROR_CHECK(esp_netif_deinit());
    esp_netif_destroy(netif_);
    netif_ = nullptr;
    state_ = state::uninitialized;
  }

  static void ip_event_handler(void* self, esp_event_base_t event_base, int32_t event_id, void* event_data);
  static void wifi_event_handler(void* self, esp_event_base_t event_base, int32_t event_id, void* event_data);
};

constexpr auto make_handler(mod& wifi) {
  using namespace requests;

  return msg::handler{msg::callback<start_service>([&](auto&&) mutable {
                        LOG_DEBUG("[WIFI] start service");
                        wifi.start();
                      }),
                      msg::callback<stop_service>([&](auto&&) mutable {
                        LOG_DEBUG("[WIFI] stop service");
                        wifi.stop();
                      }),
                      msg::callback<start_scan>([](auto&&) {
                        LOG_DEBUG("[WIFI] start scan");
                        constexpr auto conf = ::wifi_scan_config_t{};
                        constexpr auto block = false;
                        ESP_ERROR_CHECK(esp_wifi_scan_start(&conf, block));
                      }),
                      msg::callback<connect_to_ssid>([&](const auto& req) {
                        LOG_DEBUG("[WIFI] connect to ssid {}", req.ssid);
                        wifi.connect(req.ssid, req.pass);
                      }),
                      msg::callback<set_hostname>([&](const auto& req) {
                        LOG_DEBUG("[WIFI] set hostname {}", req.hostname);
                        wifi.set_hostname(req.hostname);
                      })};
}
}    // namespace wifi
