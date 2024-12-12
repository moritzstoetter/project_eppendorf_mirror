/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  @date 2024-12-10 (created)
 *  @author Moritz Stötter (moritz@modernembedded.tech)
 *  @copyright (c) Eppendorf SE 2024 - Polaris Project
 *  @brief Prototypical decoder and aggregate of all request_t's.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

#include <optional>
#include <variant>

#include "app/modules/sys/sys.hpp"
#include "app/modules/wifi/wifi.hpp"
#include "util/meta.hpp"
#include "util/net.hpp"

constexpr std::string_view SSID = "<put-your-data-here>";
constexpr std::string_view PASS = "<put-your-data-here>";
constexpr std::string_view HOSTNAME = "<put-your-data-here>";
constexpr auto IP = net::ipv4(192, 168, 0, 2);
constexpr auto NETMASK = net::ipv4(255, 255, 255, 0);
constexpr auto GW = net::ipv4(192, 168, 0, 1);

using request_t = meta::concat_t<sys::request_t, wifi::request_t>;

template<>
struct msg::decoder<request_t> {
  [[nodiscard]] constexpr auto operator()(uint8_t byte) const noexcept -> std::optional<request_t> {
    switch (byte) {
    case 'q':
      return sys::requests::dummy{};
    case 'a':
      return wifi::requests::start_service{};
    case 's':
      return wifi::requests::stop_service{};
    case 'd':
      return wifi::requests::start_scan{};
    case 'f':
      return wifi::requests::connect_to_ssid{SSID, PASS};
    case 'g':
      return wifi::requests::set_hostname{HOSTNAME};
    case 'h':
      return wifi::requests::request_ip{};
    case 'j':
      return wifi::requests::request_ip{esp_netif_ip_info_t{
        .ip = IP,
        .netmask = NETMASK,
        .gw = GW,
      }};
    default:
      return std::nullopt;
    }
  }
};    // namespace msg
