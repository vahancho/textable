# A plain text tables generator

[![Latest release](https://img.shields.io/github/v/release/vahancho/textable?include_prereleases)](https://github.com/vahancho/textable/releases)
[![Build and test (CMake)](https://github.com/vahancho/textable/actions/workflows/cmake.yml/badge.svg)](https://github.com/vahancho/textable/actions/workflows/cmake.yml)
[![Build status](https://ci.appveyor.com/api/projects/status/dey9nhcsubmtaq0g?svg=true)](https://ci.appveyor.com/project/vahancho/textable)
[![codecov](https://codecov.io/gh/vahancho/textable/branch/master/graph/badge.svg)](https://codecov.io/gh/vahancho/textable)

A C++ implementation of a plain text tables generator. The name *Textable* originates
from the combination of two words *Text* and *Table*: `Text + Table = Textable`.

```
+---------+---------+---------+
| Column0 | Column1 | Column2 |
+---------+---------+---------+
|0        |1        |2        |
+---------+---------+---------+
|        0|        1|        2|
+---------+---------+---------+
|    0    |    1    |    2    |
+---------+---------+---------+
```

## Overview

The process of the text table generation is simple and intuitive (see examples below).
You create an instance of the `Textable` class and populate it with your data.
You may add data in any order you want - `Textable` will handle it.
The cell content should be specified along with the alignment. A table can be output
to a character stream or converted to a string with `Textable::toString()` function.

## Features

- An intuitive API to build tables and manage their data
- Ability to add not only string cell values, but values of any type (convertible to string),
- Optimized table cell values storage
- Text alignment support: `Left`, `Right` and `Center`
- Supports Unicode strings
- *C++11* support

## Installation

Basically there is installation required - just compile */src/textable.h(.cpp)* in
your project and use `Textable` class. Otherwise create and use a CMake generated
installation package.

### Integration with CMake projects

However, if you use `CMake` and want to integrate the library into your project you
might want to install it first by invoking a `CMake` command from the build directory:

```
cmake --install . --prefix=<install_path>
```

Once the library is installed you can use it from in your project by adjusting the
corresponding `CMake` script. For example:

```
[..]
find_package(textable REQUIRED)

add_executable(example main.cpp)
target_link_libraries(example textable)
[..]
```

## Prerequisites

No special requirements except *C++11* compliant compiler. The class is tested latest
Ubuntu and Windows versions. For more details see the CI badges (*AppVeyor CI and GitHub actions*).

### Unicode Strings

In order to properly handle Unicode content all input strings must be UTF-8 encoded.
The client should set a proper locale too. For example: `std::setlocale(LC_ALL, "en_US.utf8");`.

## Generated Table Examples

```
+-------------+----------------+----------------+------+
|      1      |      1.2       |   Cell text    |  0   |
+-------------+----------------+----------------+------+
|      0      |       1        |       2        | 1.1  |
+-------------+----------------+----------------+------+
|    first    |     second     |     third      | 2.2  |
+-------------+----------------+----------------+------+
|    Fünf     | Двадцать пять  |    Հայաստան    |      |
+-------------+----------------+----------------+------+
|             | A Single Value |                |      |
+-------------+----------------+----------------+------+
|    true     |     false      |                |      |
+-------------+----------------+----------------+------+
| height: 1.8 |  price: 1.234  | length: 5.4321 |      |
+-------------+----------------+----------------+------+
|Left         |           Right|      Center    | four |
+-------------+----------------+----------------+------+
```

## API Usage Examples:

To add a single row with three cells:
```cpp
Textable textable;
textable.setCell(0, 0, Textable::Align::Left, 1);
textable.setCell(0, 1, Textable::Align::Right, 1.2);
textable.setCell(0, 2, Textable::Align::Center, "Cell text");
```

To add a single rows in one go:
```cpp
Textable textable;
textable.setRow(0, Textable::Align::Center, std::vector<int>{ 0, 1, 2 });
textable.setRow(1, Textable::Align::Center, std::vector<std::string>{ "first", "second", "third" });
textable.setRow(2, Textable::Align::Center, 1, 2.2f, 3.3, "Five days");
```

To set a column values in one go
```cpp
Textable textable;
textable.setColumn(3, Textable::Align::Left, std::vector<double>{ 0.0, 1.1, 2.2 });
textable.setColumn(4, Textable::Align::Left, std::vector<bool>{ true, false });
textable.setColumn(5, Textable::Align::Left, 11, 2.22, 3.0f, "Apple");
```

Usage of a custom type as a cell data
```cpp
struct TableObject
{
    float m_price;
    std::string m_title;

    friend std::ostream &operator<<(std::ostream &os, const TableObject &table);
};

std::ostream &operator<<(std::ostream &os, const TableObject &table)
{
    os << table.m_title << table.m_price;
    return os;
}

[..]

Textable textable;
textable.setRow(0, Textable::Align::Center,
                std::vector<TableObject>{ {1.80f, "height: "}, {1.234f, "price: "}, {5.4321f, "length: "} });
```

Export a table
```cpp
Textable textable;
textable.setCell(0, 0, Textable::Align::Center, 1);
textable.setCell(0, 1, Textable::Align::Center, 1.2);
textable.setCell(0, 2, Textable::Align::Center, "Cell text");

std::cout << textable;
```

## Building and testing

There are unit tests provided for the `Textable` class. You can find them in the *test/* directory.
To run them you have to build and run the test application (`ENABLE_TESTING` CMake flag set to `True`).
Please note that unit tests depend from `GTest`. Therefore it should be also present.

To build things you should invoke the following commands from the terminal,
assuming that the compiler and the environment are already configured and you are in the source directory:

### Linux (gcc + `CMake`)

```
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DENABLE_TESTING=True -DTARGET_ARCH=x64
cmake --build . --config Release
ctest
```
In order to build a debug version use `-DCMAKE_BUILD_TYPE=Debug` parameter.
To build library for 32-bit architecture use `TARGET_ARCH=x86` option instead.

### Windows

```
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=C:\build\googletest\build -DENABLE_TESTING=True -A x64
cmake --build . --config Release
ctest -C Release
```

To build library for 32-bit architecture use `-A Win32` option instead.


By default the script is configured to build a shared library. In order to build a static one use
`BUILD_SHARED_LIBS` flag set to `False`.
