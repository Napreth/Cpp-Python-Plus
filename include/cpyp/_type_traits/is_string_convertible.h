// Copyright (c) Napreth (github.com/Napreth)
// SPDX-License-Identifier: MIT

#pragma once
#include "../_config"
#include <string>
#include <type_traits>

CPYP_DETAIL_BEGIN

// This namespace is shared with `_utility/convert_from_string.h`
namespace _string_convert_detail {
    template <typename T>
    struct _is_stox_arithmetic : std::false_type {};

    template <> struct _is_stox_arithmetic<short> : std::true_type {};
    template <> struct _is_stox_arithmetic<int> : std::true_type {};
    template <> struct _is_stox_arithmetic<long> : std::true_type {};
    template <> struct _is_stox_arithmetic<long long> : std::true_type {};

    template <> struct _is_stox_arithmetic<unsigned short> : std::true_type {};
    template <> struct _is_stox_arithmetic<unsigned int> : std::true_type {};
    template <> struct _is_stox_arithmetic<unsigned long> : std::true_type {};
    template <> struct _is_stox_arithmetic<unsigned long long> : std::true_type {};

    template <> struct _is_stox_arithmetic<float> : std::true_type {};
    template <> struct _is_stox_arithmetic<double> : std::true_type {};
    template <> struct _is_stox_arithmetic<long double> : std::true_type {};

    template <typename T, typename = void>
    struct _is_string_constructible : std::false_type {};

    template <typename T>
    struct _is_string_constructible<T, std::void_t<
        decltype(T(std::declval<const std::string&>()))
        >> : std::true_type {};

    template <typename T, typename = void>
    struct _is_string_assignable : std::false_type {};

    template <typename T>
    struct _is_string_assignable<T, std::void_t<
        decltype(std::declval<T&>() = std::declval<const std::string&>())
        >> : std::true_type {};
} // namespace _string_convert_detail

template <typename T>
struct _is_string_convertible
    : std::bool_constant<
    _string_convert_detail::_is_stox_arithmetic<T>::value ||
    _string_convert_detail::_is_string_constructible<T>::value ||
    (std::is_default_constructible_v<T> &&
        _string_convert_detail::_is_string_assignable<T>::value)
    > {
};

template <typename T>
inline constexpr bool _is_string_convertible_v = _is_string_convertible<T>::value;

CPYP_DETAIL_END
