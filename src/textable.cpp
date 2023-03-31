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

size_t Textable::stringSize(const std::string &string)
{
#if defined (_MSC_VER)
    size_t size = 0;
    if (mbstowcs_s(&size, NULL, 0, string.c_str(), 0) == 0) {
        size--; // Consider the null-terminator too.
    }
    return size;
#else
    return std::mbstowcs(NULL, string.c_str(), string.size());
#endif
}

Textable::RowNumber Textable::rowCount() const
{
    return m_table.size();
}

Textable::ColumnNumber Textable::columnCount() const
{
    auto longestRow = std::max_element(m_table.cbegin(), m_table.cend(),
                                       [](const Row &rowLeft, const Row &rowRight) {
                                           return rowLeft.size() < rowRight.size();
                                       });
    return longestRow != m_table.cend() ? longestRow->size() : Textable::ColumnNumber{};
}

std::string Textable::cellData(RowNumber row, ColumnNumber column) const
{
    if (row < rowCount()) {
        const auto &rowObj = m_table.at(row);
        if (column < rowObj.size()) {
            return rowObj.at(column).m_data;
        }
    }
    return {};
}

std::string Textable::toString() const
{
    return toString(*this);
}
