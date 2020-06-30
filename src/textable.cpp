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

#include <algorithm>
#include <sstream>
#include <cassert>
#include <tuple>

static Textable::ColumnNumber s_currentColumn = {};
static Textable::RowNumber s_currentRow = {};

template<typename T>
std::string toString(T && value)
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

    m_table.at(row) = newRow;
}

template <typename T>
void Textable::setRow(T)
{
    // Do nothing.
}

template<typename Value, typename... Ts>
void Textable::setRow(RowNumber row, Value && value, Ts &&... restValues)
{
    setCell(row, s_currentColumn++, std::forward<Value>(value));
    // The recursive call.
    setRow(row, std::forward<Ts>(restValues)...);

    // Reset the counter.
    s_currentColumn = {};
}

template<typename Value, typename... Ts>
void Textable::setColumn(ColumnNumber column, Value && value, Ts &&... restValues)
{
    setCell(s_currentRow++, column, std::forward<Value>(value));
    // The recursive call.
    setColumn(column, std::forward<Ts>(restValues)...);

    // Reset the counter.
    s_currentRow = {};
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
        m_table.resize(columnData.size());
    }

    for (decltype(columnData.size()) c = 0; c < columnData.size(); ++c) {
        auto &row = m_table.at(c);
        if (column + 1 > row.size()) {
            row.resize(column + 1);
        }

        row.at(column) = toString(columnData.at(c));
    }
}

Textable::RowNumber Textable::rowCount() const
{
    return m_table.size();
}

Textable::ColumnNumber Textable::columnCount() const
{
    // Return the longest row size.
    return std::max_element(m_table.begin(), m_table.end(),
                            [](const Row &rowLeft, const Row &rowRight) {
                                return rowLeft.size() < rowRight.size();
                            })->size();
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
            static const auto offset = 2U;
            const auto valueSize = row.at(c).size() + offset;
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
            const auto spaceCount = columnWidths.at(c) - value.size();
            const auto leftSpace = spaceCount / 2;
            const auto rightSpace = spaceCount - leftSpace;
            os << std::string(leftSpace , ' ') << value << std::string(rightSpace, ' ') << '|';
        }
        os << '\n';

        drawLine();
    }
    return os;
}
