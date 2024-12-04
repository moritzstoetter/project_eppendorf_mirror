#pragma once

#include <polaris/core/developer_logging/logging.hpp>
#include <polaris/core/developer_logging/developer_logger.hpp>
#include <polaris/core/developer_logging/i_logging_sink.hpp>

#include "driver/gpio.h"
#include "driver/uart.h"

struct esp_uart_config_t {
  uart_config_t config;
  uart_intr_config_t irq_config;
  size_t rx_buffer_size;
  size_t tx_buffer_size = 0;
  struct {
    gpio_num_t tx = GPIO_NUM_NC;
    gpio_num_t rx = GPIO_NUM_NC;
    gpio_num_t rts = GPIO_NUM_NC;
    gpio_num_t cts = GPIO_NUM_NC;
  } pins;
};

namespace detail {

template<uart_port_t id, esp_uart_config_t conf>
struct esp_uart_log_sink final : polaris::core::i_logging_sink {
  esp_uart_log_sink() {
    static_assert(conf.rx_buffer_size > UART_HW_FIFO_LEN(id));

    ESP_ERROR_CHECK(uart_driver_install(id, conf.rx_buffer_size, conf.tx_buffer_size, 0, nullptr, 0));
    ESP_ERROR_CHECK(uart_param_config(id, &conf.config));
    ESP_ERROR_CHECK(uart_set_pin(id, conf.pins.tx, conf.pins.rx, conf.pins.rts, conf.pins.cts));

    auto& logger = reinterpret_cast<polaris::core::developer_logger&>(polaris::core::get_developer_logger());
    logger.add_logging_sink(*this);
  }

  void write(std::string_view data) override {
    uart_write_bytes(id, data.data(), data.size());
  }

  bool is_subscribed_to(polaris::core::DeveloperLoggingSeverity severity) override {
    return true;    // we are a greedy sink
  }
};

}    // namespace detail

using log_config = detail::esp_uart_log_sink<UART_NUM_0,
                                             esp_uart_config_t{.config{
                                                                 .baud_rate = 115'200,
                                                                 .data_bits = UART_DATA_8_BITS,
                                                                 .parity = UART_PARITY_DISABLE,
                                                                 .stop_bits = UART_STOP_BITS_1,
                                                                 .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
                                                                 .source_clk = UART_SCLK_DEFAULT,
                                                               },
                                                               .rx_buffer_size = 256,
                                                               .pins{
                                                                 .tx = GPIO_NUM_21,
                                                                 .rx = GPIO_NUM_20,
                                                               }}>;
