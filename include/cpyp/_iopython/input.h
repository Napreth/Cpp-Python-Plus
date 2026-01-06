// Copyright (c) Napreth (github.com/Napreth)
// SPDX-License-Identifier: MIT

#pragma once
#include "../_config"
#include "../_type_traits/is_string_convertible.h"
#include "../_utility/convert_from_string.h"
#include <string>
#include <iostream>
#include <stdexcept>
#include <limits>

CPYP_DETAIL_BEGIN

template <typename T>
std::enable_if_t<_is_string_convertible_v<T>, T>
input(const std::string& prompt = "") noexcept(false) {
    if (!std::cin) {
        std::cin.clear();
    }

    std::string line;
    std::cout << prompt;
    std::cout.flush();
    std::getline(std::cin, line);

    if (std::cin.eof() && line.empty()) {
        throw std::runtime_error("EOFError");
    }
    else if (std::cin.fail()) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::runtime_error("IOError: std::cin is fail.");
    }
    else if (std::cin.bad()) {
        throw std::runtime_error("IOError: std::cin is bad.");
    }

    if constexpr (std::is_same_v<T, std::string>) {
        return line;
    }
    else {
        return _convert_from_string<T>(line);
    }
}

CPYP_DETAIL_END
