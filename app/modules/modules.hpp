#pragma once

#include <optional>
#include <variant>

#include "app/modules/sys/sys.hpp"
#include "app/modules/wifi/wifi.hpp"

namespace detail {
/**
 * @template concat
 * @brief Concatenates typenames
 */
template<typename...>
struct concat;

template<typename... Args>
using concat_t = typename concat<Args...>::type;

template<template<typename...> class L, typename... Ts, typename... Us>
struct concat<L<Ts...>, L<Us...>> {
  using type = L<Ts..., Us...>;
};

template<typename L1, typename L2, typename... Rest>
struct concat<L1, L2, Rest...> {
  using type = concat_t<concat_t<L1, L2>, Rest...>;
};
}    // namespace detail

using request_t = detail::concat_t<sys::request_t, wifi::request_t>;

template<>
struct msg::decoder<request_t> {
  [[nodiscard]] constexpr auto operator()(uint8_t byte) const noexcept -> std::optional<request_t> {
    switch (byte) {
    case 'q':
      return sys::requests::dummy{};
    case 'w':
      return sys::requests::unhandled{};
    case 'a':
      return wifi::requests::start_service{};
    case 's':
      return wifi::requests::stop_service{};
    case 'd':
      return wifi::requests::start_scan{};
    default:
      return std::nullopt;
    }
  }
};    // namespace msg
