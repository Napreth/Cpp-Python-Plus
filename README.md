
<div align="center">

# CPYP: C++的Python扩展

**CPP = Cpp Python Plus**

[English](./README_en.md) | [简体中文](./README.md)

</div>

## 项目介绍

CPYP（Cpp Python Plus）旨在利用现代 C++（C++17 及以上）的一些能力，让 C++ 拥有更接近 Python 的开发体验：更顺手的控制台交互、更直观的工具函数与常用能力封装等。

当前项目仍处于开发的早期阶段，API 与目录组织可能会发生变化。

## 功能

目前已实现：

- `print()`：Python 风格的输出函数
- `input()`：Python 风格的输入函数

#### `print()` 用法

默认行为类似 Python：参数间用空格分隔、末尾换行、输出到 `std::cout`，并默认 `flush`。

选项参数需以 {列表} 形式传入，参数顺序可任意调整，但end参数必须置于sep参数之后；若flush参数位于首位，则仅支持 bool 原生类型，不允许隐式类型转换。

所有输出参数必须满足 std::ostream << arg 语法兼容要求，否则会触发编译期错误。

```cpp
#include <cpyp/iopython.h>
#include <iostream>
using namespace cpyp;
using namespace std;

int main() {
    print("hello", 123, 4.5);
    print({", ", "; "}, "a", "b", "c");                  // 自定义 sep 和 end
    print({cerr, false}, "There is something wrong.");   // 输出到 cerr 且不 flush
}
```

#### `input()` 用法

`input<T>(prompt)` 会先输出提示字符串 `prompt`，再从 `std::cin` 读取一整行（`std::getline`），并尝试转换为 `T`。

支持的 `T`：

- 数值类型：`short/int/long/long long`、对应的无符号版本、`float/double/long double`
- `std::string`
- 以及满足以下任一条件的自定义类型：可用 `T(std::string)` 构造，或默认构造后可 `obj = std::string`

```cpp
#include <cpyp/iopython.h>
using namespace cpyp;

int main() {
    auto a = input<int>("The first number: ");
    auto b = input<int>("The second number: ");
    print("Sum is", a + b);
}
```

异常说明：

- EOF 且读到空行：抛出 `std::runtime_error("EOFError")`
- 输入流 `fail/bad`：抛出 `std::runtime_error`（带具体信息）
- 转换失败：可能抛出 `std::invalid_argument` / `std::out_of_range` / `std::runtime_error`

## 环境要求

语言标准：C++17 或更高

## 使用方式

这是一个 header-only 库：把 `include/` 加入你的工程包含目录即可

导入方式：

```cpp
#include <cpyp/iopython.h>
```

## 开发计划

1. 为 `print()` 加上 STL 容器的 Python 风格输出

```text
Example:
vector/list: [1, 2, 3]
tuple: ('a', 45)
map: {'a': 1, 'b': 2}
```

2. `algorithm` 模块：Python 风格的 `max`, `sorted` 等函数


## 项目结构

- `include/cpyp/`：库头文件目录（含功能入口：`iopython.h`）
- `include/cpyp/_iopython/`：输入输出实现
- `include/cpyp/_type_traits/`：类型萃取
- `include/cpyp/_utility/`：工具函数

## LICENSE

MIT 许可证
