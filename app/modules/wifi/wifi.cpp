#include "wifi.hpp"

auto wifi::mod::wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) -> void {
    switch (event_id) {
    case WIFI_EVENT_WIFI_READY: {
        LOG_DBG("WIFI_EVENT_WIFI_READY");
    } break;
    case WIFI_EVENT_SCAN_DONE: {
        LOG_DBG("WIFI_EVENT_SCAN_DONE");
    } break;
    case WIFI_EVENT_STA_START: {
        LOG_DBG("WIFI_EVENT_STA_START");
    } break;
    case WIFI_EVENT_STA_STOP: {
        LOG_DBG("WIFI_EVENT_STA_STOP");
    } break;
    case WIFI_EVENT_STA_CONNECTED: {
        LOG_DBG("WIFI_EVENT_STA_CONNECTED");
    } break;
    case WIFI_EVENT_STA_DISCONNECTED: {
        LOG_DBG("WIFI_EVENT_STA_DISCONNECTED");
    } break;
    case WIFI_EVENT_STA_AUTHMODE_CHANGE: {
        LOG_DBG("WIFI_EVENT_STA_AUTHMODE_CHANGE");
    } break;
    case WIFI_EVENT_STA_WPS_ER_SUCCESS: {
        LOG_DBG("WIFI_EVENT_STA_WPS_ER_SUCCESS");

        if (event_data) {
            auto const *evt = static_cast<wifi_event_sta_wps_er_success_t *>(event_data);
            /* If multiple AP credentials are received from WPS, connect with first one */
            wifi_config_t ap_cred;
            memcpy(ap_cred.sta.ssid, evt->ap_cred[0].ssid, sizeof(evt->ap_cred[0].ssid));
            memcpy(ap_cred.sta.password, evt->ap_cred[0].passphrase, sizeof(evt->ap_cred[0].passphrase));
            LOG_INF("Connecting to SSID: {}, Passphrase: {}", ap_cred.sta.ssid, ap_cred.sta.password);
            ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &ap_cred));
        }
        /*
         * If only one AP credential is received from WPS, there will be no event data and
         * esp_wifi_set_config() is already called by WPS modules for backward compatibility
         * with legacy apps. So directly attempt connection here.
         */
        ESP_ERROR_CHECK(esp_wifi_wps_disable());
        esp_wifi_connect();
    } break;
    case WIFI_EVENT_STA_WPS_ER_FAILED: {
        LOG_DBG("WIFI_EVENT_STA_WPS_ER_FAILED");
    } break;
    case WIFI_EVENT_STA_WPS_ER_TIMEOUT: {
        LOG_DBG("WIFI_EVENT_STA_WPS_ER_TIMEOUT");
    } break;
    case WIFI_EVENT_STA_WPS_ER_PIN: {
        LOG_DBG("WIFI_EVENT_STA_WPS_ER_PIN");
    } break;
    case WIFI_EVENT_STA_WPS_ER_PBC_OVERLAP: {
        LOG_DBG("WIFI_EVENT_STA_WPS_ER_PBC_OVERLAP");
    } break;
    case WIFI_EVENT_AP_START: {
        LOG_DBG("WIFI_EVENT_AP_START");
    } break;
    case WIFI_EVENT_AP_STOP: {
        LOG_DBG("WIFI_EVENT_AP_STOP");
    } break;
    case WIFI_EVENT_AP_STACONNECTED: {
        LOG_DBG("WIFI_EVENT_AP_STACONNECTED");
    } break;
    case WIFI_EVENT_AP_STADISCONNECTED: {
        LOG_DBG("WIFI_EVENT_AP_STADISCONNECTED");
    } break;
    case WIFI_EVENT_AP_PROBEREQRECVED: {
        LOG_DBG("WIFI_EVENT_AP_PROBEREQRECVED");
    } break;
    case WIFI_EVENT_FTM_REPORT: {
        LOG_DBG("WIFI_EVENT_FTM_REPORT");
    } break;
    case WIFI_EVENT_STA_BSS_RSSI_LOW: {
        LOG_DBG("WIFI_EVENT_STA_BSS_RSSI_LOW");
    } break;
    case WIFI_EVENT_ACTION_TX_STATUS: {
        LOG_DBG("WIFI_EVENT_ACTION_TX_STATUS");
    } break;
    case WIFI_EVENT_ROC_DONE: {
        LOG_DBG("WIFI_EVENT_ROC_DONE");
    } break;
    case WIFI_EVENT_STA_BEACON_TIMEOUT: {
        LOG_DBG("WIFI_EVENT_STA_BEACON_TIMEOUT");
    } break;
    case WIFI_EVENT_CONNECTIONLESS_MODULE_WAKE_INTERVAL_START: {
        LOG_DBG("WIFI_EVENT_CONNECTIONLESS_MODULE_WAKE_INTERVAL_START");
    } break;
    case WIFI_EVENT_AP_WPS_RG_SUCCESS: {
        LOG_DBG("WIFI_EVENT_AP_WPS_RG_SUCCESS");
    } break;
    case WIFI_EVENT_AP_WPS_RG_FAILED: {
        LOG_DBG("WIFI_EVENT_AP_WPS_RG_FAILED");
    } break;
    case WIFI_EVENT_AP_WPS_RG_TIMEOUT: {
        LOG_DBG("WIFI_EVENT_AP_WPS_RG_TIMEOUT");
    } break;
    case WIFI_EVENT_AP_WPS_RG_PIN: {
        LOG_DBG("WIFI_EVENT_AP_WPS_RG_PIN");
    } break;
    case WIFI_EVENT_AP_WPS_RG_PBC_OVERLAP: {
        LOG_DBG("WIFI_EVENT_AP_WPS_RG_PBC_OVERLAP");
    } break;
    case WIFI_EVENT_ITWT_SETUP: {
        LOG_DBG("WIFI_EVENT_ITWT_SETUP");
    } break;
    case WIFI_EVENT_ITWT_TEARDOWN: {
        LOG_DBG("WIFI_EVENT_ITWT_TEARDOWN");
    } break;
    case WIFI_EVENT_ITWT_PROBE: {
        LOG_DBG("WIFI_EVENT_ITWT_PROBE");
    } break;
    case WIFI_EVENT_ITWT_SUSPEND: {
        LOG_DBG("WIFI_EVENT_ITWT_SUSPEND");
    } break;
    case WIFI_EVENT_TWT_WAKEUP: {
        LOG_DBG("WIFI_EVENT_TWT_WAKEUP");
    } break;
    case WIFI_EVENT_BTWT_SETUP: {
        LOG_DBG("WIFI_EVENT_BTWT_SETUP");
    } break;
    case WIFI_EVENT_BTWT_TEARDOWN: {
        LOG_DBG("WIFI_EVENT_BTWT_TEARDOWN");
    } break;
    case WIFI_EVENT_NAN_STARTED: {
        LOG_DBG("WIFI_EVENT_NAN_STARTED");
    } break;
    case WIFI_EVENT_NAN_STOPPED: {
        LOG_DBG("WIFI_EVENT_NAN_STOPPED");
    } break;
    case WIFI_EVENT_NAN_SVC_MATCH: {
        LOG_DBG("WIFI_EVENT_NAN_SVC_MATCH");
    } break;
    case WIFI_EVENT_NAN_REPLIED: {
        LOG_DBG("WIFI_EVENT_NAN_REPLIED");
    } break;
    case WIFI_EVENT_NAN_RECEIVE: {
        LOG_DBG("WIFI_EVENT_NAN_RECEIVE");
    } break;
    case WIFI_EVENT_NDP_INDICATION: {
        LOG_DBG("WIFI_EVENT_NDP_INDICATION");
    } break;
    case WIFI_EVENT_NDP_CONFIRM: {
        LOG_DBG("WIFI_EVENT_NDP_CONFIRM");
    } break;
    case WIFI_EVENT_NDP_TERMINATED: {
        LOG_DBG("WIFI_EVENT_NDP_TERMINATED");
    } break;
    case WIFI_EVENT_HOME_CHANNEL_CHANGE: {
        LOG_DBG("WIFI_EVENT_HOME_CHANNEL_CHANGE");
    } break;
    }
}

auto wifi::mod::ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) -> void {
    switch (event_id) {
    case IP_EVENT_STA_GOT_IP: {
        LOG_DBG("IP_EVENT_STA_GOT_IP");
    } break;
    case IP_EVENT_STA_LOST_IP: {
        LOG_DBG("IP_EVENT_STA_LOST_IP");
    } break;
    case IP_EVENT_AP_STAIPASSIGNED: {
        LOG_DBG("IP_EVENT_AP_STAIPASSIGNED");
    } break;
    case IP_EVENT_GOT_IP6: {
        LOG_DBG("IP_EVENT_GOT_IP6");
    } break;
    case IP_EVENT_ETH_GOT_IP: {
        LOG_DBG("IP_EVENT_ETH_GOT_IP");
    } break;
    case IP_EVENT_ETH_LOST_IP: {
        LOG_DBG("IP_EVENT_ETH_LOST_IP");
    } break;
    case IP_EVENT_PPP_GOT_IP: {
        LOG_DBG("IP_EVENT_PPP_GOT_IP");
    } break;
    case IP_EVENT_PPP_LOST_IP: {
        LOG_DBG("IP_EVENT_PPP_LOST_IP");
    } break;
    }
}