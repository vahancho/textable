# A plain text tables generator

[![Build Status](https://travis-ci.org/vahancho/textable.svg?branch=master)](https://travis-ci.org/vahancho/textable)
[![Build status](https://ci.appveyor.com/api/projects/status/dey9nhcsubmtaq0g?svg=true)](https://ci.appveyor.com/project/vahancho/textable)
[![codecov](https://codecov.io/gh/vahancho/textable/branch/master/graph/badge.svg)](https://codecov.io/gh/vahancho/textable)

A C++ implementation of a plain text tables generator. The name *Textable* originates from the combination of two words *Text* and *Table*.

## Overview

The process of the text table generation is simple and intuitive (see examples below). You create an instance of the `Textable` class and populate it with your data. You may add data in any order you want - `Textable` will handle it. By default all cell content is center aligned. A table can be output to a character stream.

## Features

- An intuitive API to build tables and manage their data
- Ability to add not only string cell values, but values of any type,
- Optimized table cell values storage
- Supports Unicode strings
- *C++11* support

## Integration

No installation required. Just compile */src/textable.h(.cpp)* in your project and use `Textable` class.

## Prerequisites

No special requirements except *C++11* compliant compiler. The class is tested with *gcc 4.8.4* and *MSVC 15.0* (Visual Studio 2017).
For more details see the CI badges (*Travis CI & AppVeyor CI*).

### Unicode Strings

In order to properly handle Unicode content all input strings must be UTF-8 encoded. The client should set a proper locale too. For example: `std::setlocale(LC_ALL, "en_US.utf8");`.

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
|      1      |      2.2       |      3.3       | four |
+-------------+----------------+----------------+------+
```

## API Usage Examples:

To add a single row with three cells:
```cpp
Textable textable;
textable.setCell(0, 0, 1);
textable.setCell(0, 1, 1.2);
textable.setCell(0, 2, "Cell text");
```

To add a single rows in one go:
```cpp
Textable textable;
textable.setRow(0, std::vector<int>{ 0, 1, 2 });
textable.setRow(1, std::vector<std::string>{ "first", "second", "third" });
textable.setRow(2, 1, 2.2f, 3.3, "Five days");
```

To set a column values in one go
```cpp
Textable textable;
textable.setColumn(3, std::vector<double>{ 0.0, 1.1, 2.2 });
textable.setColumn(4, std::vector<bool>{ true, false });
textable.setColumn(5, 11, 2.22, 3.0f, "Apple");
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
textable.setRow(0, std::vector<TableObject>{ {1.80f, "height: "}, {1.234f, "price: "}, {5.4321f, "length: "} });
```

Export a table
```cpp
Textable textable;
textable.setCell(0, 0, 1);
textable.setCell(0, 1, 1.2);
textable.setCell(0, 2, "Cell text");

std::cout << textable;
```

## Test

There are unit tests provided for `Textable` class. You can find them in the *test/* directory.
To run them you have to build and run the test application. For doing that you must invoke the following
commands from the terminal, assuming that compiler and environment are already configured and you are in the source directory:

##### Linux (gcc + cmake)

```
mkdir build
cd build
cmake ..
make
./textable_test
```

##### Linux (gcc)

```
g++ -std=c++11 -coverage -Isrc -Iinclude test/main.cpp -o textable_test
./textable_test
```

##### Windows

```
mkdir build
cd build
cmake .. -G "NMake Makefiles"
nmake
textable_test
```
