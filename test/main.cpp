/**********************************************************************************
*  MIT License                                                                    *
*                                                                                 *
*  Copyright (c) 2020 Vahan Aghajanyan <vahancho@gmail.com>                       *
*                                                                                 *
*  Permission is hereby granted, free of charge, to any person obtaining a copy   *
*  of this software and associated documentation files (the "Software"), to deal  *
*  in the Software without restriction, including without limitation the rights   *
*  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
*  copies of the Software, and to permit persons to whom the Software is          *
*  furnished to do so, subject to the following conditions:                       *
*                                                                                 *
*  The above copyright notice and this permission notice shall be included in all *
*  copies or substantial portions of the Software.                                *
*                                                                                 *
*  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
*  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
*  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
*  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
*  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
*  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
*  SOFTWARE.                                                                      *
***********************************************************************************/

#include "textable.h"
#include "textable.cpp"

#define STRINGIFY_HELPER(x) #x
#define STRINGIFY(x) STRINGIFY_HELPER(x)

#define LOCATION std::string(__FILE__ ":" STRINGIFY(__LINE__))

template<typename T,
         typename std::enable_if <!std::is_floating_point<T>{}, int>::type = 0>
bool compare(T && actual, T && expected)
{
    return actual == expected;
}

template<typename T,
         typename std::enable_if <std::is_floating_point<T>{}, int>::type = 0>
bool compare(T && actual, T && expected)
{
    static const T epsilon = 0.000001;
    // A simple comparison of floating point numbers.
    return std::abs(expected - actual) < epsilon;
}

template<typename T>
void Test(T && actual, T && expected, std::string && location)
{
    if (compare(std::forward<T>(expected), std::forward<T>(actual))) {
        std::cout << "OK:"     << '\t' << expected << " and " << actual << " are equal" << '\n';
    } else {
        std::cout << "FAILED:" << '\t' << "Expected " << expected << " but got " << actual
                  << " : " << location << '\n';
    }
}

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


int main(int, char**)
{
    Textable textable;

    textable.setCell(0, 0, 1);
    textable.setCell(0, 1, 1.2);
    textable.setCell(0, 2, "Cell text");
    Test(textable.rowCount(), 1U, LOCATION);
    Test(textable.columnCount(), 3U, LOCATION);

    textable.setRow(1, std::vector<int>{ 0, 1, 2 });
    Test(textable.rowCount(), 2U, LOCATION);

    textable.setRow(2, std::vector<std::string>{ "first", "second", "third" });
    Test(textable.rowCount(), 3U, LOCATION);

    std::vector<std::string> container{ "first", "second", "third" };
    textable.setRow(3, std::move(container));
    Test(textable.rowCount(), 4U, LOCATION);

    textable.setColumn(3, std::vector<double>{ 0.0, 1.1, 2.2 });
    Test(textable.rowCount(), 4U, LOCATION);
    Test(textable.columnCount(), 4U, LOCATION);

    textable.setCell(4, 1, "A Single Value");
    Test(textable.rowCount(), 5U, LOCATION);
    Test(textable.columnCount(), 4U, LOCATION);

    textable.setRow(5, std::vector<bool>{ true, false });
    Test(textable.rowCount(), 6U, LOCATION);
    Test(textable.columnCount(), 4U, LOCATION);

    textable.setRow(6, std::vector<TableObject>{ {1.80f, "height: "},
                                                 {1.234f, "price: "},
                                                 {5.4321f, "length: "} });
    Test(textable.rowCount(), 7U, LOCATION);
    Test(textable.columnCount(), 4U, LOCATION);

    textable.setRow(7, 1, 2.2f, 3.3, "four", TableObject{ 12.29f, "Distance: " });
    Test(textable.rowCount(), 8U, LOCATION);
    Test(textable.columnCount(), 5U, LOCATION);

    textable.setColumn(4, 1, 2.2f, 3.3, 4.4, "five", TableObject{2.29f, "Distance: "});
    Test(textable.rowCount(), 8U, LOCATION);
    Test(textable.columnCount(), 5U, LOCATION);

    std::cout << textable;

    return 0;
}
