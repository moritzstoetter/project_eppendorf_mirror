/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  @date 2024-12-10 (created)
 *  @author Moritz Stötter (moritz@modernembedded.tech)
 *  @copyright (c) Eppendorf SE 2024 - Polaris Project
 *  @brief Networking related utilities
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

#include <cstdint>

namespace net {
using ipv4_t = uint32_t;

[[nodiscard]] constexpr uint32_t ipv4(uint8_t a, uint8_t b, uint8_t c, uint8_t d) noexcept {
  return static_cast<uint32_t>(d << 24) | static_cast<uint32_t>(c << 16) | static_cast<uint32_t>(b << 8) |
         static_cast<uint32_t>(a);
}
}