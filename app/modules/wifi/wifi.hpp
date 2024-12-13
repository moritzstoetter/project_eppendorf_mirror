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

#include "esp_netif.h"
#include "esp_wifi.h"

#include "app/log_config.hpp"
#include "app/messaging/dispatch.hpp"
#include "util/string.hpp"
#include "app/modules/wifi/wifi_dbg.hpp"

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
struct request_ip {
  std::optional<esp_netif_ip_info_t> ip_config = {};
};
}    // namespace requests

// clang-format off
using request_t =
  std::variant<
    requests::start_service,
    requests::stop_service,
    requests::start_scan,
    requests::connect_to_ssid,
    requests::set_hostname,
    requests::request_ip
>;
// clang-format on

class mod {
  public:
  enum struct state { uninitialized, off, on, connected, got_ip };

  void start();
  void stop();
  void set_hostname(std::string_view hostname) const;
  void connect(std::string_view ssid, std::string_view pass) const;
  void start_dhcp() const ;
  void set_ip(const esp_netif_ip_info_t& ip_info) const;

  private:
  static void ip_event_handler(void* self, esp_event_base_t event_base, int32_t event_id, void* event_data);
  static void wifi_event_handler(void* self, esp_event_base_t event_base, int32_t event_id, void* event_data);

  void init();
  void deinit();

  constexpr static auto default_ip_conf = esp_netif_ip_info_t{};
  constexpr static auto default_netif_inherent_conf = esp_netif_inherent_config{
    .flags = static_cast<esp_netif_flags_t>(ESP_NETIF_DEFAULT_ARP_FLAGS | ESP_NETIF_DEFAULT_MLDV6_REPORT_FLAGS |
                                            ESP_NETIF_FLAG_EVENT_IP_MODIFIED),
    .mac = {},
    .ip_info = &default_ip_conf,
    .get_ip_event = IP_EVENT_STA_GOT_IP,
    .lost_ip_event = IP_EVENT_STA_LOST_IP,
    .if_key = "wifi_sta",
    .if_desc = "wifi_sta",
    .route_prio = 100,
    .bridge_info = nullptr};

  state state_ = state::uninitialized;
  esp_netif_t* netif_ = nullptr;
  esp_event_handler_instance_t ip_event_handler_instance_ = nullptr;
  esp_event_handler_instance_t wifi_event_handler_instance_ = nullptr;
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
                      }),
                      msg::callback<request_ip>([&](const auto& msg) {
                        LOG_DEBUG("[WIFI] request ip: {}", msg.ip_config);
                        if (msg.ip_config) {
                          wifi.set_ip(*msg.ip_config);
                        } else {
                          wifi.start_dhcp();
                        }
                      })};
}
}    // namespace wifi
