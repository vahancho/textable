/**********************************************************************************
*  MIT License                                                                    *
*                                                                                 *
*  Copyright (c) 2018 Vahan Aghajanyan <vahancho@gmail.com>                       *
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

#ifndef __TEXTABLE_H__
#define __TEXTABLE_H__

#include "export.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

//! Implements a textual table abstraction.
/*!
    A text table represents a table-like structure that can be streamed out as
    a plain text. A table has cells that hold string values, each of which can be referred by
    a row and column numbers.

    The process of the text table generation is simple and intuitive.
    You create an instance of the `Textable` class and populate it with your data.
    You may add data in any order you want - `Textable` will handle it.
    By default all cell content is center aligned. A table can be output to a
    character stream.

    No special requirements except C++11 compliant compiler.
    In order to properly handle Unicode content all input strings must be UTF-8
    encoded. The client should set a proper locale too. For example:
    'std::setlocale(LC_ALL, "en_US.utf8");'.
*/
class TEXTABLE_EXPORT Textable
{
public:
    using Row          = std::vector<std::string>;
    using Table        = std::vector<Row>;
    using RowNumber    = Row::size_type;
    using ColumnNumber = Table::size_type;

    //! Sets a value to the cell referred by the given \p row and \p column.
    /*!
        If table doesn't have the referred cell a new row and/or column will be added.
        If such cell already exists, the \p value will be overwritten.
        \param row    The row number
        \param column The column number
        \param value  A value. The type should be convertible to string.
    */
    template<typename T>
    void setCell(RowNumber row, ColumnNumber column, T && value);

    //! Sets a complete row values.
    /*!
        Allows to set up a complete row at once. The \p rowData is a container
        of the cell values. Normally it should be one of the standard container types
        like std::vector.
        \param row     The row number
        \param rowData A container whose elements should be convertible to string.
        \example
            Textable textable;
            textable.setRow(1, std::vector<std::string>{ "first", "second", "third" });
    */
    template<typename T, typename U = typename std::decay<decltype(*begin(std::declval<T>()))>::type>
    void setRow(RowNumber row, T && rowData);

    //! Populates a row with values of arbitrary types.
    /*!
        The number of the function arguments and their types is not defined.
        \param row The row number
        \example
            Textable textable;
            textable.setRow(7, 1, 2.2f, 3.3, "four");
    */
    template<typename Value, typename... Ts>
    void setRow(RowNumber row, Value && value, Ts &&... restValues);

    //! Sets a complete column values.
    /*!
        Allows to set up a complete table column at once. The \p columnData is a container
        of cell values. Normally it should be one of the standard container types
        like std::vector.
        \param column     The column number
        \param columnData A container whose elements should be convertible to string.
        \example
            Textable textable;
            textable.setColumn(3, std::vector<double>{ 0.0, 1.1, 2.2 });
    */
    template<typename T, typename U = typename std::decay<decltype(*begin(std::declval<T>()))>::type>
    void setColumn(ColumnNumber column, T && columnData);

    //! Populates a column with values of arbitrary types.
    /*!
        The number of the function arguments and their types is not defined.
        \param column The column number
        \example
            Textable textable;
            textable.setColumn(12, 1, 2.2f, 3.3, "four");
    */
    template<typename Value, typename... Ts>
    void setColumn(ColumnNumber column, Value && value, Ts &&... restValues);

    //! Returns the number of rows of the table.
    RowNumber rowCount() const;

    //! Returns the number of columns of the table.
    ColumnNumber columnCount() const;

    //! Returns a cell value (string) that corresponds to the given \p row and \p column.
    /*!
        \returns Returns a cell value (string) that corresponds to the given row and column or
                 an empty string if row or column are out or range.
    */
    std::string cellValue(RowNumber row, ColumnNumber column) const;

    //! Returns the string representation of the data stored in the table.
    std::string toString() const;

    friend std::ostream &operator<<(std::ostream &os, const Textable &table);

private:
    template<typename T>
    std::string toString(T && value) const;

    /// Implements the base case for setRow() variadic function template recursion.
    template <typename T>
    void setRow(T);

    /// Implements the base case for setColumn() variadic function template recursion.
    template <typename T>
    void setColumn(T);

    /// Returns the real number of string characters.
    /*!
        In cases when string stores multi byte characters, std::string::size() function
        will return the number of *bytes*, not characters.
    */
    static size_t stringSize(const std::string &string);

    Table m_table;
    Textable::ColumnNumber m_currentColumn = {};
    Textable::RowNumber m_currentRow = {};
};

////////////////////////////////////////////////////////////////////////////////
// Definition of the function templates

template<typename T>
std::string Textable::toString(T && value) const
{
    std::ostringstream stream;
    stream << std::boolalpha << std::forward<T>(value);
    return stream.str();
}

template<typename T>
void Textable::setCell(RowNumber row, ColumnNumber column, T && value)
{
    if (row + 1 > rowCount()) {
        m_table.resize(row + 1);
    }
    auto &rowObj = m_table.at(row);

    if (column + 1 > rowObj.size()) {
        rowObj.resize(column + 1);
    }

    rowObj.at(column) = toString(std::forward<T>(value));
}

// The specialization for Taxtable::Row data. We don't need to perform values conversion.
template<>
inline void Textable::setRow(RowNumber row, Textable::Row && rowData)
{
    if (row + 1 > rowCount()) {
        m_table.resize(row + 1);
    }
    m_table.at(row) = rowData;
}

template<typename T, typename U>
void Textable::setRow(RowNumber row, T && rowData)
{
    if (row + 1 > rowCount()) {
        m_table.resize(row + 1);
    }

    Textable::Row newRow;
    newRow.reserve(rowData.size());

    for (const auto &value : rowData) {
        newRow.emplace_back(toString(value));
    }

    if (m_currentColumn == 0) {
        m_table.at(row) = newRow;
    } else {
        auto &currentRow = m_table.at(row);
        currentRow.reserve(currentRow.size() + newRow.size());
        currentRow.insert(currentRow.end(), newRow.begin(), newRow.end());
    }
}

template <typename T>
void Textable::setRow(T)
{
    // Do nothing.
}

template<typename Value, typename... Ts>
void Textable::setRow(RowNumber row, Value && value, Ts &&... restValues)
{
    setCell(row, m_currentColumn++, std::forward<Value>(value));
    // The recursive call.
    setRow(row, std::forward<Ts>(restValues)...);

    // Reset the counter.
    m_currentColumn = {};
}

template<typename Value, typename... Ts>
void Textable::setColumn(ColumnNumber column, Value && value, Ts &&... restValues)
{
    setCell(m_currentRow++, column, std::forward<Value>(value));
    // The recursive call.
    setColumn(column, std::forward<Ts>(restValues)...);

    // Reset the counter.
    m_currentRow = {};
}

template <typename T>
void Textable::setColumn(T)
{
    // Do nothing.
}

template<typename T, typename U>
void Textable::setColumn(ColumnNumber column, T && columnData)
{
    if (columnData.size() > rowCount()) {
        m_table.resize(columnData.size() + m_currentRow);
    }

    for (decltype(columnData.size()) r = 0; r < columnData.size(); ++r) {
        const auto insertionRow = m_currentRow + r;
        auto &row = m_table.at(insertionRow);
        if (column + 1 > row.size()) {
            row.resize(column + 1);
        }

        row.at(column) = toString(columnData.at(r));
    }
}

std::ostream &operator<<(std::ostream &os, const Textable &table)
{
    if (table.rowCount() == 0) {
        return os;
    }

    std::vector<Textable::ColumnNumber> columnWidths(table.columnCount(), 0);

    // Find max. width for each column.
    for (const auto &row : table.m_table) {
        for (auto c = 0U; c < row.size(); ++c) {
            assert(c < columnWidths.size());
            static const auto offset = Textable::ColumnNumber(2);
            const auto valueSize = Textable::stringSize(row.at(c)) + offset;
            if (valueSize > columnWidths.at(c)) {
                columnWidths.at(c) = valueSize;
            }
        }
    }

    auto drawLine = [&]() {
        os << '+';
        for (auto c = 0U; c < table.columnCount(); ++c) {
            os << std::string(columnWidths.at(c), '-') << '+';
        }
        os << '\n';
    };

    drawLine();

    for (const auto &row : table.m_table) {
        os << '|';
        for (auto c = 0U; c < table.columnCount(); ++c) {
            const auto &value = c < row.size() ? row.at(c) : std::string{};
            const auto spaceCount = columnWidths.at(c) - Textable::stringSize(value);
            const auto leftSpace = spaceCount / 2;
            const auto rightSpace = spaceCount - leftSpace;
            os << std::string(leftSpace , ' ') << value << std::string(rightSpace, ' ') << '|';
        }
        os << '\n';

        drawLine();
    }
    return os;
}


#endif // !__TEXTABLE_H__

