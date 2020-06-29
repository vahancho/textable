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

#include <cassert>

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
    assert(textable.rowCount() == 1);
    assert(textable.columnCount() == 3);

    textable.setRow(1, std::vector<int>{ 0, 1, 2 });
    assert(textable.rowCount() == 2);

    textable.setRow(2, std::vector<std::string>{ "first", "second", "third" });
    assert(textable.rowCount() == 3);

    textable.setColumn(3, std::vector<double>{ 0.0, 1.1, 2.2 });
    assert(textable.columnCount() == 4);

    textable.setCell(3, 1, "A Single Value");
    assert(textable.rowCount() == 4);
    assert(textable.columnCount() == 4);

    textable.setRow(4, std::vector<bool>{ true, false });
    assert(textable.rowCount() == 5);
    assert(textable.columnCount() == 4);

    textable.setRow(5, std::vector<TableObject>{ {1.80f, "height: "}, {1.234f, "price: "}, {5.4321f, "length: "} });
    assert(textable.rowCount() == 6);
    assert(textable.columnCount() == 4);

    std::cout << textable;

    return 0;
}
