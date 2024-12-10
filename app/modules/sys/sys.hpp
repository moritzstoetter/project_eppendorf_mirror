/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
 *  @date 2024-12-10 (created)
 *  @author Moritz Stötter (moritz@modernembedded.tech)
 *  @copyright (c) Eppendorf SE 2024 - Polaris Project
 *  @brief Example of a module that received requests.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

#include "app/messaging/dispatch.hpp"

#include "app/log_config.hpp"

namespace sys {
namespace requests {
struct dummy {
  uint8_t data;
};

struct unhandled {};
}    // namespace requests

using request_t = std::variant<requests::dummy, requests::unhandled>;

// module
struct mod {
  constexpr mod() = default;
};

constexpr auto make_handler(mod&) {
  return msg::handler{
    msg::callback<requests::dummy>(msg::always_match,
                                   [](auto&&) {
                                     LOG_DEBUG("Dummy");
                                   }),
    msg::callback<requests::unhandled>(msg::always_match,
                                       [](auto&&) {
                                         LOG_DEBUG("Unhandled");
                                       }),
  };
}
}    // namespace sys
