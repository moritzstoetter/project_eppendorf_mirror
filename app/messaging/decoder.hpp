#pragma once

namespace msg {
template<typename>
struct decoder;

template<typename T>
constexpr auto decode = decoder<T>{};
};