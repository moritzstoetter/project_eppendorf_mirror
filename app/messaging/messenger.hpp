#pragma once

#include <chrono>
#include <optional>

#include <edf/os/thread.hpp>
#include <edf/os/queue.hpp>

#include "app/messaging/decoder.hpp"
#include "bsp/time.hpp"
#include "bsp/uart.hpp"

struct messenger_t {
  private:
  using msg_uart = uart<UART_NUM_1,
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
                                            .tx = GPIO_NUM_7,
                                            .rx = GPIO_NUM_6,
                                          }}>;

  public:
  messenger_t() :
    thread_{{"uart_msg_thd", edf::os::LowestTaskPriority}, [this] {
              worker_();
            }} {
    if (not msg_uart::init()) {
      LOG_ERROR("Failed to init uart");
    }
  }

  [[nodiscard]] std::optional<messages_t> receive(const std::chrono::milliseconds& timeout = os::forever) {
    messages_t msg;
    return out_queue_.try_pop_for(msg, timeout) ? std::optional{msg} : std::nullopt;
  }

  private:
  [[noreturn]] void worker_() {
    while (true) {
      const auto msg = msg_uart::rx().and_then([](auto raw) {
        return decode<messages_t>(raw);
      });

      if (msg.has_value())
        out_queue_.push(*msg);
    }
  }

  edf::os::static_queue<messages_t, 10> out_queue_;
  edf::os::static_thread<1'024 * 10> thread_;
};
