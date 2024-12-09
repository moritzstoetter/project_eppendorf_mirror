#pragma once

#include "app/messaging/dispatch.hpp"

#include "log_config.hpp"

namespace sys {
namespace messages {
struct dummy {
    uint8_t data;
};

struct unhandled {};
}

using messages_t = std::variant<messages::dummy, messages::unhandled>;

// module
struct mod {
    constexpr mod() = default;
};

constexpr auto make_handler(mod &) {
    return msg::handler{
        msg::callback<messages::dummy>(
            msg::always_match,
            [](auto &&) { LOG_DEBUG("Dummy"); }),
        msg::callback<messages::unhandled>(
            msg::always_match,
            [](auto &&) { LOG_DEBUG("Unhandled"); }),
    };
}
} // namespace sys
