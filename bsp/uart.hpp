#pragma once

#include "driver/gpio.h"
#include "driver/uart.h"

#include "bsp/time.hpp"

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

template<auto id, auto conf>
struct uart {
  static bool init() noexcept {
    static_assert(conf.rx_buffer_size > UART_HW_FIFO_LEN(id));

    return ESP_OK == uart_driver_install(id, conf.rx_buffer_size, conf.tx_buffer_size, 0, nullptr, 0) and
           ESP_OK == uart_param_config(id, &conf.config) and
           ESP_OK == uart_set_pin(id, conf.pins.tx, conf.pins.rx, conf.pins.rts, conf.pins.cts);
  }

  template<std::ranges::contiguous_range R>
  static void tx(const R& tx_payload) noexcept {
    uart_write_bytes(
      id, std::bit_cast<uint8_t*>(std::begin(tx_payload)), std::size(tx_payload) * sizeof(typename R::value_type));
  }

  [[nodiscard]] static std::optional<uint8_t>
    rx(const std::chrono::milliseconds& timeout = os::forever) noexcept {
    uint8_t buf;
    return uart_read_bytes(id, &buf, 1, os::to_ticks(timeout)) == 1 ? std::optional{buf} : std::nullopt;
  }
};
