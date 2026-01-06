// Copyright (c) Napreth (github.com/Napreth)
// SPDX-License-Identifier: MIT

#pragma once
#include "../_config"
#include "../_type_traits/is_string_convertible.h"

#include <string>
#include <type_traits>
#include <cstdlib>
#include <stdexcept>
#include <limits>
#include <typeinfo>
#include <string_view>
#include <cmath>
#include <sstream>
#include <charconv>
#include <array>
#include <cerrno>

CPYP_DETAIL_BEGIN

// This namespace is in `_type_traits/is_string_convertible.h`
namespace _string_convert_detail {
    template <typename T>
    constexpr bool _is_within_signed_range(long long val) noexcept {
        static_assert(std::is_signed_v<T> && std::is_integral_v<T>, "T must be signed integer");
        return val >= static_cast<long long>(std::numeric_limits<T>::min()) &&
            val <= static_cast<long long>(std::numeric_limits<T>::max());
    }

    template <typename T>
    constexpr bool _is_within_unsigned_range(unsigned long long val) noexcept {
        static_assert(std::is_unsigned_v<T> && std::is_integral_v<T>, "T must be unsigned integer");
        return val <= static_cast<unsigned long long>(std::numeric_limits<T>::max());
    }

    template <typename T>
    constexpr bool _is_within_float_range(long double val) noexcept {
        static_assert(std::is_floating_point_v<T>, "T must be floating point");
        return (val >= static_cast<long double>(std::numeric_limits<T>::lowest()) &&
            val <= static_cast<long double>(std::numeric_limits<T>::max())) &&
            !std::isnan(val) && !std::isinf(val);
    }

    template <typename T>
    std::string _float_to_string(T val) {
        static_assert(std::is_floating_point_v<T>, "T must be floating point");
        std::array<char, 128> buf;
        auto [ptr, ec] = std::to_chars(buf.data(), buf.data() + buf.size(), val,
            std::chars_format::general,
            std::numeric_limits<T>::max_digits10);
        if (ec == std::errc{}) {
            return std::string(buf.data(), ptr - buf.data());
        }
        std::ostringstream oss;
        oss.precision(std::numeric_limits<T>::max_digits10);
        oss << val;
        return oss.str();
    }
}  // namespace _string_convert_detail

template <typename T>
std::enable_if_t<_string_convert_detail::_is_stox_arithmetic<T>::value, T>
_convert_from_string(const std::string& str) {
    try {
        if constexpr (std::is_floating_point_v<T>) {
            long double val;
            if constexpr (std::is_same_v<T, float>) {
                val = std::stof(str);
            }
            else if constexpr (std::is_same_v<T, double>) {
                val = std::stod(str);
            }
            else {
                val = std::stold(str);
            }

            if (!_string_convert_detail::_is_within_float_range<T>(val)) {
                std::string min_str = _string_convert_detail::_float_to_string(std::numeric_limits<T>::lowest());
                std::string max_str = _string_convert_detail::_float_to_string(std::numeric_limits<T>::max());
                throw std::out_of_range("Value out of range for " + std::string(typeid(T).name()) +
                    " (min: " + min_str + ", max: " + max_str + ")");
            }
            return static_cast<T>(val);
        }
        else if constexpr (std::is_signed_v<T> && std::is_integral_v<T>) {
            char* endptr = nullptr;
            errno = 0;
            const long long val = std::strtoll(str.c_str(), &endptr, 10);

            if (endptr == str.c_str()) {
                throw std::invalid_argument("No valid digits in string: \"" + str + "\"");
            }
            if (*endptr != '\0') {
                throw std::invalid_argument("Trailing invalid characters: \"" + str + "\"");
            }
            if (errno == ERANGE || !_string_convert_detail::_is_within_signed_range<T>(val)) {
                throw std::out_of_range(
                    "Value " + std::to_string(val) + " out of range for " + std::string(typeid(T).name()) +
                    " (min: " + std::to_string(std::numeric_limits<T>::min()) +
                    ", max: " + std::to_string(std::numeric_limits<T>::max()) + ")"
                );
            }
            return static_cast<T>(val);
        }
        else if constexpr (std::is_unsigned_v<T> && std::is_integral_v<T>) {
            char* endptr = nullptr;
            errno = 0;
            const unsigned long long val = std::strtoull(str.c_str(), &endptr, 10);

            if (endptr == str.c_str()) {
                throw std::invalid_argument("No valid digits in string: \"" + str + "\"");
            }
            if (*endptr != '\0') {
                throw std::invalid_argument("Trailing invalid characters: \"" + str + "\"");
            }
            if (errno == ERANGE || !_string_convert_detail::_is_within_unsigned_range<T>(val)) {
                throw std::out_of_range(
                    "Value " + std::to_string(val) + " out of range for " + std::string(typeid(T).name()) +
                    " (max: " + std::to_string(std::numeric_limits<T>::max()) + ")"
                );
            }
            return static_cast<T>(val);
        }
    }
    catch (const std::out_of_range& e) {
        throw std::out_of_range(
            "Convert error (out of range): \"" + str + "\" to " + std::string(typeid(T).name()) + " - " + e.what()
        );
    }
    catch (const std::invalid_argument& e) {
        throw std::invalid_argument(
            "Convert error (invalid format): \"" + str + "\" to " + std::string(typeid(T).name()) + " - " + e.what()
        );
    }
    catch (...) {
        throw std::runtime_error("Convert error (unknown): \"" + str + "\" to " + std::string(typeid(T).name()));
    }
}

template <typename T>
std::enable_if_t<_string_convert_detail::_is_stox_arithmetic<T>::value, T>
_convert_from_string(std::string_view str) {
    return _convert_from_string<T>(std::string(str));
}

template <typename T>
std::enable_if_t<_string_convert_detail::_is_string_constructible<T>::value && !std::is_arithmetic_v<T>, T>
_convert_from_string(const std::string& str) {
    static_assert(_is_string_convertible_v<T>,
        "Type T is not convertible from std::string (no valid conversion logic)");
    try {
        return T(str);
    }
    catch (const std::exception& e) {
        throw std::invalid_argument(
            "Convert error: \"" + str + "\" to " + std::string(typeid(T).name()) + " - " + e.what()
        );
    }
    catch (...) {
        throw std::runtime_error("Convert error (unknown): \"" + str + "\" to " + std::string(typeid(T).name()));
    }
}

template <typename T>
std::enable_if_t<_string_convert_detail::_is_string_constructible<T>::value && !std::is_arithmetic_v<T>, T>
_convert_from_string(std::string_view str) {
    return _convert_from_string<T>(std::string(str));
}

template <typename T>
std::enable_if_t<
    std::is_default_constructible_v<T>&&
    _string_convert_detail::_is_string_assignable<T>::value &&
    !_string_convert_detail::_is_string_constructible<T>::value &&
    !std::is_arithmetic_v<T>,
    T>
    _convert_from_string(const std::string& str) {
    static_assert(_is_string_convertible_v<T>,
        "Type T is not convertible from std::string (no valid conversion logic)");
    try {
        T obj;
        obj = str;
        return obj;
    }
    catch (const std::exception& e) {
        throw std::invalid_argument(
            "Convert error: \"" + str + "\" to " + std::string(typeid(T).name()) + " - " + e.what()
        );
    }
    catch (...) {
        throw std::runtime_error("Convert error (unknown): \"" + str + "\" to " + std::string(typeid(T).name()));
    }
}

template <typename T>
std::enable_if_t<
    std::is_default_constructible_v<T>&&
    _string_convert_detail::_is_string_assignable<T>::value &&
    !_string_convert_detail::_is_string_constructible<T>::value &&
    !std::is_arithmetic_v<T>,
    T>
    _convert_from_string(std::string_view str) {
    return _convert_from_string<T>(std::string(str));
}

CPYP_DETAIL_END
