// Copyright (c) Napreth (github.com/Napreth)
// SPDX-License-Identifier: MIT

#pragma once

#if defined(_MSC_VER)
#  define CPYP_COMPILER_MSVC
#elif defined(__clang__)
#  define CPYP_COMPILER_CLANG
#elif defined(__GNUC__)
#  define CPYP_COMPILER_GCC
#endif
