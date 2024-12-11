/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  @date 2024-12-10 (created)
 *  @author Moritz Stötter (moritz@modernembedded.tech)
 *  @copyright (c) Eppendorf SE 2024 - Polaris Project
 *  @brief Formatter specializations for wifi related types.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

#include "esp_wifi.h"

template<>
class emio::formatter<::wifi_auth_mode_t> : public formatter<std::string_view> {
  public:
  constexpr result<void> format(writer& out, wifi_auth_mode_t arg) const noexcept {
    switch (arg) {
    case WIFI_AUTH_OPEN:
      return out.write_str("WIFI_AUTH_OPEN");
    case WIFI_AUTH_WEP:
      return out.write_str("WIFI_AUTH_WEP");
    case WIFI_AUTH_WPA_PSK:
      return out.write_str("WIFI_AUTH_WPA_PSK");
    case WIFI_AUTH_WPA2_PSK:
      return out.write_str("WIFI_AUTH_WPA2_PSK");
    case WIFI_AUTH_WPA_WPA2_PSK:
      return out.write_str("WIFI_AUTH_WPA_WPA2_PSK");
    case WIFI_AUTH_ENTERPRISE:
      return out.write_str("WIFI_AUTH_ENTERPRISE");
    case WIFI_AUTH_WPA3_PSK:
      return out.write_str("WIFI_AUTH_WPA3_PSK");
    case WIFI_AUTH_WPA2_WPA3_PSK:
      return out.write_str("WIFI_AUTH_WPA2_WPA3_PSK");
    case WIFI_AUTH_WAPI_PSK:
      return out.write_str("WIFI_AUTH_WAPI_PSK");
    case WIFI_AUTH_OWE:
      return out.write_str("WIFI_AUTH_OWE");
    case WIFI_AUTH_WPA3_ENT_192:
      return out.write_str("WIFI_AUTH_WPA3_ENT_192");
    case WIFI_AUTH_WPA3_EXT_PSK:
      return out.write_str("WIFI_AUTH_WPA3_EXT_PSK");
    case WIFI_AUTH_WPA3_EXT_PSK_MIXED_MODE:
      return out.write_str("WIFI_AUTH_WPA3_EXT_PSK_MIXED_MODE");
    case WIFI_AUTH_DPP:
      return out.write_str("WIFI_AUTH_DPP");
    case WIFI_AUTH_MAX:
      return out.write_str("WIFI_AUTH_MAX");
    default:
      return out.write_str("unknown");
    }
  }
};

template<>
class emio::formatter<::wifi_ap_record_t> : public formatter<std::string_view> {
  public:
  constexpr result<void> format(writer& out, const ::wifi_ap_record_t& arg) const noexcept {
    return format_to(out,
                     "scan result: {}, rssi {}, pchan {}, schan {}, authmode {}",
                     reinterpret_cast<const char*>(arg.ssid),
                     arg.rssi,
                     arg.primary,
                     arg.second,
                     arg.authmode);
  }
};

template <typename T>
class emio::formatter<std::optional<T>> {
public:
  constexpr result<void> parse(reader& format_rdr) noexcept {
    return underlying_.parse(format_rdr);
  }

  constexpr result<void> format(writer& out, const std::optional<T>& arg) const noexcept {
    if (!arg.has_value()) {
      return out.write_str(detail::sv("none"));
    } else {
      EMIO_TRYV(out.write_str(detail::sv("optional(")));
      EMIO_TRYV(underlying_.format(out, *arg));
      return out.write_char(')');
    }
  }

private:
  formatter<T> underlying_;
};

template<>
class emio::formatter<esp_ip4_addr> : public formatter<std::string_view> {
public:
  constexpr result<void> format(writer& out, const esp_ip4_addr& arg) const noexcept {
    return format_to(out, "{}.{}.{}.{}", arg.addr >> 24 & 0xFF, arg.addr >> 16 & 0xFF, arg.addr >> 8 & 0xFF, arg.addr & 0xFF);
  }
};

template<>
class emio::formatter<esp_netif_ip_info_t> : public formatter<std::string_view> {
  public:
  constexpr result<void> format(writer& out, const esp_netif_ip_info_t& arg) const noexcept {
    return format_to(out, "ip {}, netmask {}, gw {}", arg.ip, arg.netmask, arg.gw);
  }
};
