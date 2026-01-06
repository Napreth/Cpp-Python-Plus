
<div align="center">

# CPYP: Python Extensions for C++

**CPP = Cpp Python Plus**

[English](./README_en.md) | [简体中文](./README.md)

</div>

## Introduction

CPYP (Cpp Python Plus) aims to leverage some capabilities of modern C++ (C++17 and above) to give C++ a development experience closer to Python: more convenient console interaction, more intuitive utility functions, and common capability wrappers.

This project is still in an early stage of development. The API and directory organization may change.

## Features

Currently implemented:

- `print()`: a Python-style output function
- `input()`: a Python-style input function

#### `print()` Usage

The default behavior is similar to Python: arguments are separated by spaces, a newline is appended at the end, output goes to `std::cout`, and it `flush`es by default.

Optional parameters must be passed in the form of a {list}. The argument order can be adjusted freely, but the `end` parameter must be placed after the `sep` parameter; if the `flush` parameter is placed first, only the native `bool` type is supported and implicit type conversion is not allowed.

All output arguments must satisfy the `std::ostream << arg` compatibility requirement; otherwise a compile-time error will be triggered.

```cpp
#include <cpyp/iopython.h>
#include <iostream>
using namespace cpyp;
using namespace std;

int main() {
		print("hello", 123, 4.5);
		print({", ", "; "}, "a", "b", "c");                  // Customize sep and end
		print({cerr, false}, "There is something wrong.");   // Output to cerr and do not flush
}
```

#### `input()` Usage

`input<T>(prompt)` first outputs the prompt string `prompt`, then reads an entire line from `std::cin` (`std::getline`), and tries to convert it to `T`.

Supported `T`:

- Numeric types: `short/int/long/long long`, their unsigned variants, and `float/double/long double`
- `std::string`
- Custom types that satisfy either of the following: constructible via `T(std::string)`, or default-constructible and then assignable via `obj = std::string`

```cpp
#include <cpyp/iopython.h>
using namespace cpyp;

int main() {
		auto a = input<int>("The first number: ");
		auto b = input<int>("The second number: ");
		print("Sum is", a + b);
}
```

Exception notes:

- EOF with an empty line read: throws `std::runtime_error("EOFError")`
- Input stream `fail/bad`: throws `std::runtime_error` (with specific information)
- Conversion failure: may throw `std::invalid_argument` / `std::out_of_range` / `std::runtime_error`

## Requirements

Language standard: C++17 or later

## Usage

This is a header-only library: just add `include/` to your project's include directories.

Include it as:

```cpp
#include <cpyp/iopython.h>
```

## Roadmap

1. Add Python-style printing for STL containers to `print()`

```text
Example:
vector/list: [1, 2, 3]
tuple: ('a', 45)
map: {'a': 1, 'b': 2}
```

2. `algorithm` module: Python-style functions such as `max`, `sorted`, etc.


## Project Structure

- `include/cpyp/`: library header directory (entry point: `iopython.h`)
- `include/cpyp/_iopython/`: input/output implementation
- `include/cpyp/_type_traits/`: type traits
- `include/cpyp/_utility/`: utility functions

## LICENSE

MIT License
