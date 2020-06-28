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

#include <iostream>
#include <vector>

class Textable
{
public:
    using RowNumber    = size_t;
    using ColumnNumber = size_t;

    template<typename T>
    void setCell(RowNumber row, ColumnNumber column, T && value);

    template<typename T>
    void setRow(RowNumber row, T && rowData);

    template<typename T>
    void setColumn(ColumnNumber column, T && columnData);

    RowNumber rowCount() const;
    ColumnNumber columnCount() const;

    friend std::ostream &operator<<(std::ostream &os, const Textable &table);

private:
    using Row = std::vector<std::string>;
    using Table = std::vector<Row>;

    Table m_table;
};

#endif // !__TEXTABLE_H__

