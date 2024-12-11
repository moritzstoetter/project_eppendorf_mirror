/** * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
 *  @date 2024-12-10 (created)
 *  @author Moritz Stötter (moritz@modernembedded.tech)
 *  @copyright (c) Eppendorf SE 2024 - Polaris Project
 *  @brief Some metafunctions
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#pragma once

namespace meta {
/**
 * @template concat
 * @brief Concatenates typenames of type lists
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

/**
 * @template all_of
 * @brief concept that holds true if all of the elements of the list like L
 * satisfy Pred
 */
namespace detail {
template <typename, template <typename> class>
struct is_all_of;

template <template <typename...> class L, template <typename> class Pred, typename T>
struct is_all_of<L<T>, Pred> {
  constexpr static bool value = Pred<T>::value;
};

template <template <typename...> class L, typename T, typename... Ts, template <typename> class Pred>
struct is_all_of<L<T, Ts...>, Pred> {
  constexpr static bool value = Pred<T>::value and is_all_of<L<Ts...>, Pred>::value;
};
} // namespace detail

template <typename L, template <typename> class Pred>
concept all_of = detail::is_all_of<L, Pred>::value;
}    // namespace meta