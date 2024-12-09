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
