// Copyright (c) Napreth (github.com/Napreth)
// SPDX-License-Identifier: MIT

#pragma once
#include "./compiler.h"

#ifdef CPYP_COMPILER_MSVC
#  define CPYP_CPLUSPLUS _MSVC_LANG
#elif defined(__cplusplus)
#  define CPYP_CPLUSPLUS __cplusplus
#endif

#if defined(CPYP_CPLUSPLUS)
#  if CPYP_CPLUSPLUS < 201103L
#    define CPYP_CXX03_LANG
#    warning "Cpyp only supports C++17 and later"
#  endif
#  if CPYP_CPLUSPLUS <= 201103L
#    define CPYP_STD_VER 11
#    warning "Cpyp only supports C++17 and later"
#  elif CPYP_CPLUSPLUS <= 201402L
#    define CPYP_STD_VER 14
#    warning "Cpyp only supports C++17 and later"
#  elif CPYP_CPLUSPLUS <= 201703L
#    define CPYP_STD_VER 17
#  elif CPYP_CPLUSPLUS <= 202002L
#    define CPYP_STD_VER 20
#  elif CPYP_CPLUSPLUS <= 202302L
#    define CPYP_STD_VER 23
#  else
#    define CPYP_STD_VER 26
#  endif
#endif
