/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  @date 2024-12-10 (created)
 *  @author Moritz Stötter (moritz@modernembedded.tech)
 *  @copyright (c) Eppendorf SE 2024 - Polaris Project
 *  @brief Wifi service class sources.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "wifi.hpp"

#include "esp_wps.h"

void wifi::mod::start() {
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

void wifi::mod::stop() {
  ESP_ERROR_CHECK(esp_wifi_stop());
  ESP_ERROR_CHECK(esp_event_handler_instance_unregister(IP_EVENT, ESP_EVENT_ANY_ID, &ip_event_handler_instance_));
  ESP_ERROR_CHECK(esp_event_handler_instance_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler_instance_));
  state_ = state::off;
}

void wifi::mod::set_hostname(std::string_view hostname) const {
  std::array<char, 32> buf{};
  string::copy_null_terminate(hostname, buf);
  ESP_ERROR_CHECK(esp_netif_set_hostname(netif_, buf.data()));
}

void wifi::mod::connect(std::string_view ssid, std::string_view pass) const {
  auto conf = ::wifi_config_t{};
  string::copy_null_terminate(ssid, conf.sta.ssid);
  string::copy_null_terminate(pass, conf.sta.password);
  ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &conf));
  ESP_ERROR_CHECK(esp_wifi_connect());
}

void wifi::mod::start_dhcp() const {
  ESP_ERROR_CHECK(esp_netif_dhcpc_start(netif_));
}

void wifi::mod::set_ip(const esp_netif_ip_info_t& ip_info) const {
  ESP_ERROR_CHECK(esp_netif_dhcpc_stop(netif_));
  ESP_ERROR_CHECK(esp_netif_set_ip_info(netif_, &ip_info));
}

void wifi::mod::init() {
  // esp netif
  ESP_ERROR_CHECK(esp_netif_init());
  netif_ = esp_netif_create_wifi(WIFI_IF_STA, &default_netif_inherent_conf);
  ESP_ERROR_CHECK(esp_wifi_set_default_wifi_sta_handlers());
  assert(netif_);
  ESP_ERROR_CHECK(esp_netif_attach_wifi_station(netif_));

  // esp wifi driver
  const ::wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

  state_ = state::off;
}

void wifi::mod::deinit() {
  stop();

  ESP_ERROR_CHECK(esp_wifi_deinit());
  esp_netif_deinit();    // not checking the error because this call is not supported yet, and therefore returns
  // ESP_ERR_NOT_SUPPORTED
  esp_netif_destroy(netif_);
  netif_ = nullptr;
  state_ = state::uninitialized;
}

void wifi::mod::wifi_event_handler(void* self,
                                   [[maybe_unused]] esp_event_base_t event_base,
                                   int32_t event_id,
                                   void* event_data) {
  auto* wifi = static_cast<mod*>(self);

  switch (event_id) {
  case WIFI_EVENT_WIFI_READY: {
    LOG_DEBUG("WIFI_EVENT_WIFI_READY");
  } break;
  case WIFI_EVENT_SCAN_DONE: {
    LOG_DEBUG("WIFI_EVENT_SCAN_DONE");

    ::wifi_ap_record_t scan_result;
    while (esp_wifi_scan_get_ap_record(&scan_result) == ESP_OK) {
      LOG_DEBUG("[WIFI] {}", scan_result);
    }
  } break;
  case WIFI_EVENT_STA_START: {
    LOG_DEBUG("WIFI_EVENT_STA_START");
  } break;
  case WIFI_EVENT_STA_STOP: {
    LOG_DEBUG("WIFI_EVENT_STA_STOP");
  } break;
  case WIFI_EVENT_STA_CONNECTED: {
    LOG_DEBUG("WIFI_EVENT_STA_CONNECTED");
    wifi->state_ = state::connected;
  } break;
  case WIFI_EVENT_STA_DISCONNECTED: {
    LOG_DEBUG("WIFI_EVENT_STA_DISCONNECTED");
    wifi->state_ = state::on;
  } break;
  case WIFI_EVENT_STA_AUTHMODE_CHANGE: {
    LOG_DEBUG("WIFI_EVENT_STA_AUTHMODE_CHANGE");
  } break;
  case WIFI_EVENT_STA_WPS_ER_SUCCESS: {
    LOG_DEBUG("WIFI_EVENT_STA_WPS_ER_SUCCESS");
    ESP_ERROR_CHECK(esp_wifi_wps_disable());
    if (event_data != nullptr) {
      const auto* evt = static_cast<::wifi_event_sta_wps_er_success_t*>(event_data);
      wifi->connect(reinterpret_cast<const char*>(&evt->ap_cred[0].ssid[0]),
                    reinterpret_cast<const char*>(&evt->ap_cred[0].passphrase[0]));
    }
  } break;
  case WIFI_EVENT_STA_WPS_ER_FAILED: {
    LOG_DEBUG("WIFI_EVENT_STA_WPS_ER_FAILED");
  } break;
  case WIFI_EVENT_STA_WPS_ER_TIMEOUT: {
    LOG_DEBUG("WIFI_EVENT_STA_WPS_ER_TIMEOUT");
  } break;
  case WIFI_EVENT_STA_WPS_ER_PIN: {
    LOG_DEBUG("WIFI_EVENT_STA_WPS_ER_PIN");
  } break;
  case WIFI_EVENT_STA_WPS_ER_PBC_OVERLAP: {
    LOG_DEBUG("WIFI_EVENT_STA_WPS_ER_PBC_OVERLAP");
  } break;
  case WIFI_EVENT_AP_START: {
    LOG_DEBUG("WIFI_EVENT_AP_START");
  } break;
  case WIFI_EVENT_AP_STOP: {
    LOG_DEBUG("WIFI_EVENT_AP_STOP");
  } break;
  case WIFI_EVENT_AP_STACONNECTED: {
    LOG_DEBUG("WIFI_EVENT_AP_STACONNECTED");
  } break;
  case WIFI_EVENT_AP_STADISCONNECTED: {
    LOG_DEBUG("WIFI_EVENT_AP_STADISCONNECTED");
  } break;
  case WIFI_EVENT_AP_PROBEREQRECVED: {
    LOG_DEBUG("WIFI_EVENT_AP_PROBEREQRECVED");
  } break;
  case WIFI_EVENT_FTM_REPORT: {
    LOG_DEBUG("WIFI_EVENT_FTM_REPORT");
  } break;
  case WIFI_EVENT_STA_BSS_RSSI_LOW: {
    LOG_DEBUG("WIFI_EVENT_STA_BSS_RSSI_LOW");
  } break;
  case WIFI_EVENT_ACTION_TX_STATUS: {
    LOG_DEBUG("WIFI_EVENT_ACTION_TX_STATUS");
  } break;
  case WIFI_EVENT_ROC_DONE: {
    LOG_DEBUG("WIFI_EVENT_ROC_DONE");
  } break;
  case WIFI_EVENT_STA_BEACON_TIMEOUT: {
    LOG_DEBUG("WIFI_EVENT_STA_BEACON_TIMEOUT");
  } break;
  case WIFI_EVENT_CONNECTIONLESS_MODULE_WAKE_INTERVAL_START: {
    LOG_DEBUG("WIFI_EVENT_CONNECTIONLESS_MODULE_WAKE_INTERVAL_START");
  } break;
  case WIFI_EVENT_AP_WPS_RG_SUCCESS: {
    LOG_DEBUG("WIFI_EVENT_AP_WPS_RG_SUCCESS");
  } break;
  case WIFI_EVENT_AP_WPS_RG_FAILED: {
    LOG_DEBUG("WIFI_EVENT_AP_WPS_RG_FAILED");
  } break;
  case WIFI_EVENT_AP_WPS_RG_TIMEOUT: {
    LOG_DEBUG("WIFI_EVENT_AP_WPS_RG_TIMEOUT");
  } break;
  case WIFI_EVENT_AP_WPS_RG_PIN: {
    LOG_DEBUG("WIFI_EVENT_AP_WPS_RG_PIN");
  } break;
  case WIFI_EVENT_AP_WPS_RG_PBC_OVERLAP: {
    LOG_DEBUG("WIFI_EVENT_AP_WPS_RG_PBC_OVERLAP");
  } break;
  case WIFI_EVENT_ITWT_SETUP: {
    LOG_DEBUG("WIFI_EVENT_ITWT_SETUP");
  } break;
  case WIFI_EVENT_ITWT_TEARDOWN: {
    LOG_DEBUG("WIFI_EVENT_ITWT_TEARDOWN");
  } break;
  case WIFI_EVENT_ITWT_PROBE: {
    LOG_DEBUG("WIFI_EVENT_ITWT_PROBE");
  } break;
  case WIFI_EVENT_ITWT_SUSPEND: {
    LOG_DEBUG("WIFI_EVENT_ITWT_SUSPEND");
  } break;
  case WIFI_EVENT_TWT_WAKEUP: {
    LOG_DEBUG("WIFI_EVENT_TWT_WAKEUP");
  } break;
  case WIFI_EVENT_BTWT_SETUP: {
    LOG_DEBUG("WIFI_EVENT_BTWT_SETUP");
  } break;
  case WIFI_EVENT_BTWT_TEARDOWN: {
    LOG_DEBUG("WIFI_EVENT_BTWT_TEARDOWN");
  } break;
  case WIFI_EVENT_NAN_STARTED: {
    LOG_DEBUG("WIFI_EVENT_NAN_STARTED");
  } break;
  case WIFI_EVENT_NAN_STOPPED: {
    LOG_DEBUG("WIFI_EVENT_NAN_STOPPED");
  } break;
  case WIFI_EVENT_NAN_SVC_MATCH: {
    LOG_DEBUG("WIFI_EVENT_NAN_SVC_MATCH");
  } break;
  case WIFI_EVENT_NAN_REPLIED: {
    LOG_DEBUG("WIFI_EVENT_NAN_REPLIED");
  } break;
  case WIFI_EVENT_NAN_RECEIVE: {
    LOG_DEBUG("WIFI_EVENT_NAN_RECEIVE");
  } break;
  case WIFI_EVENT_NDP_INDICATION: {
    LOG_DEBUG("WIFI_EVENT_NDP_INDICATION");
  } break;
  case WIFI_EVENT_NDP_CONFIRM: {
    LOG_DEBUG("WIFI_EVENT_NDP_CONFIRM");
  } break;
  case WIFI_EVENT_NDP_TERMINATED: {
    LOG_DEBUG("WIFI_EVENT_NDP_TERMINATED");
  } break;
  case WIFI_EVENT_HOME_CHANNEL_CHANGE: {
    LOG_DEBUG("WIFI_EVENT_HOME_CHANNEL_CHANGE");
  } break;
  default: {
    LOG_WARNING("Unknown event: {}", event_id);
  }
  }
}

void wifi::mod::ip_event_handler([[maybe_unused]] void* self,
                                 [[maybe_unused]] esp_event_base_t event_base,
                                 [[maybe_unused]] int32_t event_id,
                                 [[maybe_unused]] void* event_data) {
  auto* wifi = static_cast<mod*>(self);

  switch (event_id) {
  case IP_EVENT_STA_GOT_IP: {
    LOG_DEBUG("IP_EVENT_STA_GOT_IP");
    wifi->state_ = state::got_ip;
  } break;
  case IP_EVENT_STA_LOST_IP: {
    LOG_DEBUG("IP_EVENT_STA_LOST_IP");
    wifi->state_ = state::connected;
  } break;
  case IP_EVENT_AP_STAIPASSIGNED: {
    LOG_DEBUG("IP_EVENT_AP_STAIPASSIGNED");
  } break;
  case IP_EVENT_GOT_IP6: {
    LOG_DEBUG("IP_EVENT_GOT_IP6");
  } break;
  case IP_EVENT_ETH_GOT_IP: {
    LOG_DEBUG("IP_EVENT_ETH_GOT_IP");
  } break;
  case IP_EVENT_ETH_LOST_IP: {
    LOG_DEBUG("IP_EVENT_ETH_LOST_IP");
  } break;
  case IP_EVENT_PPP_GOT_IP: {
    LOG_DEBUG("IP_EVENT_PPP_GOT_IP");
  } break;
  case IP_EVENT_PPP_LOST_IP: {
    LOG_DEBUG("IP_EVENT_PPP_LOST_IP");
  } break;
  default: {
    LOG_WARNING("Unknown event: {}", event_id);
  }
  }
}
