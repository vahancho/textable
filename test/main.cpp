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

#include <clocale>
#include <gtest/gtest.h>

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

TEST(General, AddCell)
{
    Textable textable;

    textable.setCell(0, 0, "Title");
    textable.setCell(0, 1, "Column 1");
    textable.setCell(0, 2, "Column 2");
    EXPECT_EQ(textable.rowCount(), 1);
    EXPECT_EQ(textable.columnCount(), 3);
    EXPECT_EQ(textable.cellValue(0, 0), "Title");
    EXPECT_EQ(textable.cellValue(0, 1), "Column 1");
    EXPECT_EQ(textable.cellValue(0, 2), "Column 2");
    EXPECT_EQ(textable.cellValue(0, 3), std::string{});
    EXPECT_EQ(toString(textable), "+-------+----------+----------+\n"
                                  "| Title | Column 1 | Column 2 |\n"
                                  "+-------+----------+----------+\n");
}

TEST(General, SetRowMixed)
{
    Textable textable;
    textable.setRow(1, "Numbers", std::vector<int>{ 1, 2 });
    EXPECT_EQ(textable.rowCount(), 2);
    EXPECT_EQ(textable.columnCount(), 3);
    EXPECT_EQ(textable.cellValue(1, 0), "Numbers");
    EXPECT_EQ(textable.cellValue(1, 1), "1");
    EXPECT_EQ(textable.cellValue(1, 2), "2");
    EXPECT_EQ(toString(textable), "+---------+---+---+\n"
                                  "|         |   |   |\n"
                                  "+---------+---+---+\n"
                                  "| Numbers | 1 | 2 |\n"
                                  "+---------+---+---+\n");
}

TEST(General, SetRowStrings)
{
    Textable textable;
    textable.setRow(2, std::vector<std::string>{ "Mixed", "first", "second" });
    EXPECT_EQ(textable.rowCount(), 3);
    EXPECT_EQ(textable.columnCount(), 3);
    EXPECT_EQ(textable.cellValue(2, 0), "Mixed");
    EXPECT_EQ(textable.cellValue(2, 1), "first");
    EXPECT_EQ(textable.cellValue(2, 2), "second");
    EXPECT_EQ(toString(textable), "+-------+-------+--------+\n"
                                  "|       |       |        |\n"
                                  "+-------+-------+--------+\n"
                                  "|       |       |        |\n"
                                  "+-------+-------+--------+\n"
                                  "| Mixed | first | second |\n"
                                  "+-------+-------+--------+\n");
}

TEST(General, SetRowUnicode)
{
    Textable textable;
    std::vector<std::string> container{ "Unicode", u8"Fünf", u8"Двадцать пять", u8"Հայաստան" };
    textable.setRow(3, std::move(container));
    EXPECT_EQ(textable.rowCount(), 4);
    EXPECT_EQ(textable.columnCount(), 4);
    EXPECT_EQ(textable.cellValue(3, 2), u8"Двадцать пять");
    EXPECT_EQ(textable.cellValue(3, 3), u8"Հայաստան");
    EXPECT_EQ(textable.cellValue(3, 1), u8"Fünf");
    EXPECT_EQ(textable.cellValue(3, 0), "Unicode");

    std::string expected("+---------+------+---------------+----------+\n"
                         "|         |      |               |          |\n"
                         "+---------+------+---------------+----------+\n"
                         "|         |      |               |          |\n"
                         "+---------+------+---------------+----------+\n"
                         "|         |      |               |          |\n"
                         "+---------+------+---------------+----------+\n"
                         "| Unicode | Fünf | Двадцать пять | Հայաստան |\n"
                         "+---------+------+---------------+----------+\n");
    std::cout << "Expected table:\n" << expected;
    std::cout << "\nActual table:\n" << textable << '\n';

    EXPECT_EQ(toString(textable), expected);
}

TEST(General, SetColumnMixedFloating)
{
    Textable textable;
    textable.setColumn(3, "Column 3", std::vector<double>{ 0.0, 1.1 });
    EXPECT_EQ(textable.rowCount(), 3);
    EXPECT_EQ(textable.columnCount(), 4);
    EXPECT_EQ(textable.cellValue(0, 3), "Column 3");
    EXPECT_EQ(textable.cellValue(1, 3), "0");
    EXPECT_EQ(textable.cellValue(2, 3), "1.1");
    EXPECT_EQ(toString(textable), "+--+--+--+----------+\n"
                                  "|  |  |  | Column 3 |\n"
                                  "+--+--+--+----------+\n"
                                  "|  |  |  |    0     |\n"
                                  "+--+--+--+----------+\n"
                                  "|  |  |  |   1.1    |\n"
                                  "+--+--+--+----------+\n");
}

TEST(General, SingleCell)
{
    Textable textable;
    textable.setCell(4, 1, "A Single Value");
    EXPECT_EQ(textable.rowCount(), 5);
    EXPECT_EQ(textable.columnCount(), 2);
    EXPECT_EQ(textable.cellValue(4, 1), "A Single Value");
    EXPECT_EQ(toString(textable), "+--+----------------+\n"
                                  "|  |                |\n"
                                  "+--+----------------+\n"
                                  "|  |                |\n"
                                  "+--+----------------+\n"
                                  "|  |                |\n"
                                  "+--+----------------+\n"
                                  "|  |                |\n"
                                  "+--+----------------+\n"
                                  "|  | A Single Value |\n"
                                  "+--+----------------+\n");
}

TEST(General, BoleanValues)
{
    Textable textable;
    textable.setRow(0, std::vector<bool>{ true, false });
    EXPECT_EQ(textable.rowCount(), 1);
    EXPECT_EQ(textable.columnCount(), 2);
    EXPECT_EQ(textable.cellValue(0, 0), "true");
    EXPECT_EQ(textable.cellValue(0, 1), "false");
    EXPECT_EQ(toString(textable), "+------+-------+\n"
                                  "| true | false |\n"
                                  "+------+-------+\n");
}

TEST(General, CustomObjectValues)
{
    Textable textable;
    textable.setRow(1, std::vector<TableObject>{ { 1.80f, "height: " },
                                                 { 1.234f, "price: " },
                                                 { 5.4321f, "length: " } });
    EXPECT_EQ(textable.rowCount(), 2);
    EXPECT_EQ(textable.columnCount(), 3);
    EXPECT_EQ(textable.cellValue(1, 0), "height: 1.8");
    EXPECT_EQ(textable.cellValue(1, 1), "price: 1.234");
    EXPECT_EQ(textable.cellValue(1, 2), "length: 5.4321");
    EXPECT_EQ(toString(textable), "+-------------+--------------+----------------+\n"
                                  "|             |              |                |\n"
                                  "+-------------+--------------+----------------+\n"
                                  "| height: 1.8 | price: 1.234 | length: 5.4321 |\n"
                                  "+-------------+--------------+----------------+\n");
}

TEST(General, MixedCustomObjectValues)
{
    Textable textable;
    textable.setRow(0, 1, 2.2f, 3.3, "four", TableObject{ 12.29f, "Distance: " });
    EXPECT_EQ(textable.rowCount(), 1);
    EXPECT_EQ(textable.columnCount(), 5);
    EXPECT_EQ(textable.cellValue(0, 0), "1");
    EXPECT_EQ(textable.cellValue(0, 1), "2.2");
    EXPECT_EQ(textable.cellValue(0, 2), "3.3");
    EXPECT_EQ(textable.cellValue(0, 3), "four");
    EXPECT_EQ(textable.cellValue(0, 4), "Distance: 12.29");
    EXPECT_EQ(toString(textable), "+---+-----+-----+------+-----------------+\n"
                                  "| 1 | 2.2 | 3.3 | four | Distance: 12.29 |\n"
                                  "+---+-----+-----+------+-----------------+\n");
}

TEST(General, EmptyTable)
{
    Textable textable;
    EXPECT_EQ(textable.rowCount(), 0);
    EXPECT_EQ(textable.columnCount(), 0);
    EXPECT_EQ(toString(textable), "");
}

int main(int argc, char**argv)
{
    // This is required to properly handle the multi-byte string sizes.
    std::setlocale(LC_ALL, "en_US.utf8");

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
