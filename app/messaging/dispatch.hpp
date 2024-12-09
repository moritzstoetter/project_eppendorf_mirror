#pragma once

#include <concepts>
#include <tuple>
#include <utility>

namespace msg {
inline constexpr auto always_match = [](auto&&) {
  return true;
};

namespace detail {
template<typename T, typename P, typename F>
struct callback {
  constexpr auto operator()(const T& msg) const noexcept(noexcept(std::declval<F>()) and noexcept(std::declval<P>()))
    -> bool {
    if (pred(msg)) {
      action(msg);
      return true;
    }
    return false;
  }

  [[no_unique_address]] P pred;
  [[no_unique_address]] mutable F action;
};

template<typename Msg>
struct callback_construct_t {
  template<std::invocable<Msg> P, std::invocable<Msg> F>
    requires std::same_as<std::invoke_result_t<P, Msg>, bool>
  [[nodiscard]] constexpr auto operator()(P&& p, F&& f) const {
    return callback<Msg, std::remove_cvref_t<P>, std::remove_cvref_t<F>>{std::forward<P>(p), std::forward<F>(f)};
  }

  template<std::invocable<Msg> F>
  [[nodiscard]] constexpr auto operator()(F&& f) const {
    return callback<Msg, decltype(always_match), std::remove_cvref_t<F>>{always_match, std::forward<F>(f)};
  }
};
}    // namespace detail

template<typename Msg>
inline constexpr auto callback = detail::callback_construct_t<Msg>{};

template<typename... Callbacks>
struct handler {
  constexpr explicit handler(Callbacks&&... cbs) : callbacks_{std::forward_as_tuple(cbs...)} {}

  template<typename Msg>
  constexpr auto operator()(const Msg& msg) const -> bool {
    bool hndld = false;
    std::apply(
      [&msg, &hndld]<typename... Cbs>(Cbs&... cbs) {
        (..., [&] {
          if constexpr (std::invocable<Cbs, Msg>)
            hndld = cbs(msg) or hndld;
        }());
      },
      callbacks_);
    return hndld;
  }

  private:
  std::tuple<Callbacks...> callbacks_;
};

template<typename... Callbacks>
handler(Callbacks...) -> handler<Callbacks...>;
}    // namespace msg
