/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *  @date 2024-12-10 (created)
 *  @author Moritz Stötter (moritz@modernembedded.tech)
 *  @copyright (c) Eppendorf SE 2024 - Polaris Project
 *  @brief Networking related utilities
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

#include <algorithm>
#include <ranges>

#pragma once

namespace string {
template<std::ranges::input_range In, std::ranges::output_range<std::ranges::range_value_t<In>> Out>
constexpr void copy_null_terminate(In&& in, Out&& out) noexcept {
  auto [_, out_it] =
    std::ranges::copy_n(std::begin(in), std::clamp(std::size(in), 0UZ, std::size(out) - 1), std::begin(out));
  *out_it = '\0';
}
}