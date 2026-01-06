// Copyright (c) Napreth (github.com/Napreth)
// SPDX-License-Identifier: MIT

#pragma once
#include "../_config"
#include <iostream>
#include <type_traits>

CPYP_DETAIL_BEGIN

template <typename T, typename = void>
struct _is_printable : std::false_type {};

template <typename T>
struct _is_printable<T,
    std::void_t<decltype(std::declval<std::ostream&>() << std::declval<T>())>>
: std::true_type {};

template <typename T>
inline constexpr bool _is_printable_v = _is_printable<T>::value;

CPYP_DETAIL_END
