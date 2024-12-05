#pragma once

#include <polaris/core/developer_logging/logging.hpp>
#include <polaris/core/developer_logging/developer_logger.hpp>
#include <polaris/core/developer_logging/i_logging_sink.hpp>

#include "bsp/uart.hpp"

struct esp_uart_log_sink final : polaris::core::i_logging_sink {
  private:
  using log_uart = uart<UART_NUM_0,
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

  public:
  esp_uart_log_sink() {
    log_uart::init();

    auto& logger = reinterpret_cast<polaris::core::developer_logger&>(polaris::core::get_developer_logger());
    logger.add_logging_sink(*this);
    // lifetime !
  }

  void write(std::string_view data) override {
    log_uart::tx(data);
  }

  bool is_subscribed_to(polaris::core::DeveloperLoggingSeverity severity) override {
    return true;    // we are a greedy sink
  }
};
