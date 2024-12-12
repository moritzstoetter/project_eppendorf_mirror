/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  @date 2024-12-10 (created)
 *  @author Moritz Stötter (moritz@modernembedded.tech)
 *  @copyright (c) Eppendorf SE 2024 - Polaris Project
 *  @brief Message handler and callback templates.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

#include <concepts>
#include <tuple>
#include <utility>

namespace msg {
constexpr auto always_match = [](auto&&) {
  return true;
};

namespace detail {
template<typename T, std::predicate<T> P, std::invocable<T> F>
struct callback {
  template<typename U>
  struct handles {
    constexpr static bool value = std::same_as<T, U>;
  };

  constexpr bool operator()(const T& msg) const noexcept(noexcept(std::declval<F>()) and noexcept(std::declval<P>())) {
    if (pred(msg)) {
      action(msg);
      return true;
    }
    return false;
  }

  [[no_unique_address]] P pred;
  [[no_unique_address]] mutable F action;
};

template<typename T>
struct callback_construct_t {
  template<std::predicate<T> P, std::invocable<T> F>
    requires std::same_as<std::invoke_result_t<P, T>, bool>
  [[nodiscard]] constexpr auto operator()(P&& p, F&& f) const {
    return callback<T, std::remove_cvref_t<P>, std::remove_cvref_t<F>>{std::forward<P>(p), std::forward<F>(f)};
  }

  template<std::invocable<T> F>
  [[nodiscard]] constexpr auto operator()(F&& f) const {
    return callback<T, decltype(always_match), std::remove_cvref_t<F>>{always_match, std::forward<F>(f)};
  }
};
}    // namespace detail

template<typename T>
inline constexpr auto callback = detail::callback_construct_t<T>{};

template<typename... Callbacks>
struct handler {
  constexpr explicit handler(Callbacks&&... cbs) : callbacks_{std::forward_as_tuple(cbs...)} {}

  template<typename Msg>
  constexpr bool operator()(Msg&& msg) const {
    bool hndld = false;
    std::apply(
      [&msg, &hndld]<typename... Cbs>(Cbs&... cbs) {
        (..., [&] {
          if constexpr (std::invocable<Cbs, Msg>) {
            hndld = cbs(std::forward<Msg>(msg)) or hndld;
          }
        }());
      },
      callbacks_);
    return hndld;
  }

  template<typename T>
  struct handles {
    constexpr static bool value = (Callbacks::template handles<T>::value or ...);
  };

  private:
  std::tuple<Callbacks...> callbacks_;
};

template<typename... Callbacks>
handler(Callbacks...) -> handler<Callbacks...>;
}    // namespace msg
