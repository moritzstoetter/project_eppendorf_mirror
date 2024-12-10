#pragma once

#include <cstdint>
#include <optional>

namespace sys {
struct dummy {};
struct unhandled {};
}    // namespace sys

using messages_t = std::variant<sys::dummy, sys::unhandled>;

template<typename>
struct decoder;

template<>
struct decoder<messages_t> {
  [[nodiscard]] std::optional<messages_t> operator()(uint8_t byte) const {
    switch (byte) {
    case 'a':
      return sys::dummy{};
    case 's':
      return sys::unhandled{};
    default: {
      LOG_WARNING("Failed to decode: {:c}", byte);
      return std::nullopt;
    }
    }
  }
};

template<typename T>
constexpr auto decode = decoder<T>{};
