#pragma once

#include <cstdint>

#include <metl/os/thread.hpp>

#include "esp_wifi.h"
#include "esp_wps.h"
#include "nvs_flash.h"
#include "nvs_handle.hpp"

#include "dispatch.hpp"
#include "log_config.hpp"
#include "socket.hpp"

namespace wifi {
LOG_CONFIG("WIFI", log_level::debug);

// messages
struct start {};
struct stop {};
struct connect {};

struct start_wps {
};

struct make_server {
    uint16_t port;
};

// module
struct mod {

    using messages_t           = std::variant<start, stop, connect, make_server, start_wps>;
    constexpr static auto name = log_tag;

    constexpr static auto server_task_opts = os::thread_options{
        .name       = "server_thd",
        .stack_size = 1'024 * 50,
        .priority   = os::thread_prio::LOWEST,
        .dormant    = false,
    };

    static auto ip_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) -> void;
    static auto wifi_event_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data) -> void;

    auto make_tcp_server(uint16_t port) -> void {
        os::make_thread<server_task_opts, os::thread_policy::detached>([port = port] {
            net::tcp_server server;
            server.listen(port);
            auto conn = server.accept();

            bool stay_alive = true;
            while (conn and stay_alive) {
                stay_alive = std::visit(
                    stdx::overload_set{
                        [&](net::data &&dt) {
                            LOG_INF("[tcp:{}] Received data: {}", port, dt.payload);
                            return true;
                        },
                        [&](net::error &&er) {
                            LOG_ERR("[tcp:{}] Received error: {}", port, er.code);
                            return false;
                        },
                        [&](net::timeout &&) {
                            LOG_WRN("[tcp:{}] Timeout", port);
                            return false;
                        },
                        [&](net::remote_close &&) {
                            LOG_INF("[tcp:{}] Remote close", port);
                            return false;
                        },
                    },
                    conn->receive());
            }
        });
    }

    constexpr mod() = default;
};

constexpr auto make_handler(mod &m) {
    return msg::handler{
        msg::callback<start>(
            msg::always_match,
            [wifi = m](auto &&) mutable {
                LOG_INF("Start");
                ESP_ERROR_CHECK(esp_netif_init());
                ESP_ERROR_CHECK(esp_event_loop_create_default());
                wifi_init_config_t init_cfg = WIFI_INIT_CONFIG_DEFAULT();
                ESP_ERROR_CHECK(esp_wifi_init(&init_cfg));

                esp_netif_inherent_config_t esp_netif_config = ESP_NETIF_INHERENT_DEFAULT_WIFI_STA();
                esp_netif_config.if_desc                     = "netif";
                esp_netif_config.route_prio                  = 128;
                auto *netif                                  = esp_netif_create_wifi(WIFI_IF_STA, &esp_netif_config);
                assert(netif);
                esp_wifi_set_default_wifi_sta_handlers();

                ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));

                wifi_config_t wifi_cfg{};
                wifi_cfg.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
                wifi_cfg.sta.sae_pwe_h2e        = WPA3_SAE_PWE_UNSPECIFIED;
                ESP_ERROR_CHECK(esp_wifi_get_config(WIFI_IF_STA, &wifi_cfg));

                ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi.wifi_event_handler, nullptr));
                ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, ESP_EVENT_ANY_ID, &wifi.ip_event_handler, nullptr));

                ESP_ERROR_CHECK(esp_wifi_start());
            }),
        msg::callback<connect>(
            msg::always_match,
            [](auto &&) {
                ESP_ERROR_CHECK(esp_wifi_connect());
            }),
        msg::callback<make_server>(
            msg::always_match,
            [wifi = m](auto const &msg) mutable {
                LOG_INF("Make server, port: {}", msg.port);
                wifi.make_tcp_server(msg.port);
            }),
        msg::callback<start_wps>(
            msg::always_match,
            [wifi = m](auto &&) mutable {
                LOG_INF("WPS mode");
                esp_wps_config_t wps_cfg = WPS_CONFIG_INIT_DEFAULT(WPS_TYPE_PBC);
                ESP_ERROR_CHECK(esp_wifi_wps_enable(&wps_cfg));
                ESP_ERROR_CHECK(esp_wifi_wps_start(0));
            }),
    };
}
} // namespace wifi
