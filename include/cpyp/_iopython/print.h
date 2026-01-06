// Copyright (c) Napreth (github.com/Napreth)
// SPDX-License-Identifier: MIT

#pragma once
#include "../_config"
#include "../_type_traits/is_printable.h"

#include <iostream>
#include <string>
#include <utility>
#include <typeinfo>

CPYP_DETAIL_BEGIN

namespace _print_detail {
    inline constexpr char DEFAULT_SEP[] = " ";
    inline constexpr char DEFAULT_END[] = "\n";
    inline std::ostream& DEFAULT_OSTR = std::cout;
    inline constexpr bool DEFAULT_FLUSH = true;

    struct _print_args_package {
        const std::string sep;
        const std::string end;
        std::ostream& ostr;
        const bool flush;

        // The sep and end parameters cannot be swapped because they are of the same
        // type and indistinguishable.
        // If we consider two strings as one, then a total of A(3, 3) = 6 overloads are
        // needed.

        _print_args_package(
            const std::string s = DEFAULT_SEP,
            const std::string e = DEFAULT_END,
            std::ostream& o = DEFAULT_OSTR,
            const bool f = DEFAULT_FLUSH
        ) noexcept : sep(s), end(e), ostr(o), flush(f) {
        }

        _print_args_package(
            const std::string s,
            const std::string e,
            const bool f,
            std::ostream& o = DEFAULT_OSTR
        ) noexcept : sep(s), end(e), ostr(o), flush(f) {
        }

        _print_args_package(
            std::ostream& o,
            const std::string s = DEFAULT_SEP,
            const std::string e = DEFAULT_END,
            const bool f = DEFAULT_FLUSH
        ) noexcept : sep(s), end(e), ostr(o), flush(f) {
        }

        _print_args_package(
            std::ostream& o,
            const bool f,
            const std::string s = DEFAULT_SEP,
            const std::string e = DEFAULT_END
        ) noexcept : sep(s), end(e), ostr(o), flush(f) {
        }

        // To avoid implicit conversion of char[] to bool, which conflicts with the first constructor.
        template <typename B,
            std::enable_if_t<std::is_same_v<B, bool>, int> = 0>
        _print_args_package(
            B f,
            const std::string s = DEFAULT_SEP,
            const std::string e = DEFAULT_END,
            std::ostream& o = DEFAULT_OSTR
        ) noexcept : sep(s), end(e), ostr(o), flush(f) {
        }

        _print_args_package(
            const bool f,
            std::ostream& o,
            const std::string s = DEFAULT_SEP,
            const std::string e = DEFAULT_END
        ) noexcept : sep(s), end(e), ostr(o), flush(f) {
        }
    };

    template <typename... Args, std::size_t... Is>
    void _print_impl(_print_args_package options, std::index_sequence<Is...>, Args&&... args) noexcept(false) {
        ([&]() {
            using ArgType = std::decay_t<decltype(args)>;
            static_assert(_is_printable_v<ArgType>,
                "Arg is not printable (no operator<< for std::ostream).");

            options.ostr << args;
            if constexpr (Is != sizeof...(Args) - 1) {
                options.ostr << options.sep;
            }
            }(),
                ...);
        options.ostr << options.end;
        if (options.flush) {
            options.ostr.flush();
        }
    }
}  // namespace _print_detail

/**
 * @brief A python-like print function
 *
 * @details
 * 
 * Usage: print(arg1, arg2, ...) or print(options, arg1, arg2, ...)
 * 
 * Options: {string sep, string end, ostream& ostr, bool flush}
 * 
 * Example:
 * 
 * print(1, a + b, 1.5);
 * 
 * print({std::cerr, false}, "There is something wrong");
 *
 * @param options Print configuration parameters (sep/end/ostr/flush) corresponding to Python print keyword arguments;
 *                supports omitted/reordered params (end must follow sep); flush as first param only accepts native bool
 * 
 * @param args List of arguments to print (unlimited quantity); all arguments must be printable types that implement
 *             ostream& operator<<(ostream&, arg)
 */
template <typename... Args>
void print(_print_detail::_print_args_package options, Args&&... args) noexcept(false) {
    constexpr auto indices = std::index_sequence_for<Args...>();
    _print_detail::_print_impl(std::move(options), indices, std::forward<Args>(args)...);
}

/**
 * @brief A python-like print function
 *
 * @details
 *
 * Usage: print(arg1, arg2, ...) or print(options, arg1, arg2, ...)
 *
 * Options: {string sep, string end, ostream& ostr, bool flush}
 *
 * Example:
 *
 * print(1, a + b, 1.5);
 *
 * print({std::cerr, false}, "There is something wrong");
 *
 * @param options Print configuration parameters (sep/end/ostr/flush) corresponding to Python print keyword arguments;
 *                supports omitted/reordered params (end must follow sep); flush as first param only accepts native bool
 *
 * @param args List of arguments to print (unlimited quantity); all arguments must be printable types that implement
 *             ostream& operator<<(ostream&, arg)
 */
template <typename... Args>
void print(Args&&... args) noexcept(false) {
    constexpr auto indices = std::index_sequence_for<Args...>();
    _print_detail::_print_impl(_print_detail::_print_args_package{}, indices, std::forward<Args>(args)...);
}

CPYP_DETAIL_END
