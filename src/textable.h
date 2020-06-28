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

//! Implements a textual table abstraction.
/*!
    A text table represents a table-like structure that can be streamed out as
    a plain text. A table has cells that hold string values, each of which can be referred by
    a row and column numbers.
*/
class Textable
{
public:
    using RowNumber    = size_t;
    using ColumnNumber = size_t;

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
        of cell values.
        \param row     The row number
        \param rowData A container whose elements should be convertible to string.
    */
    template<typename T>
    void setRow(RowNumber row, T && rowData);

    //! Sets a complete column values.
    /*!
        Allows to set up a complete table column at once. The \p columnData is a container
        of cell values.
        \param column     The column number
        \param columnData A container whose elements should be convertible to string.
    */
    template<typename T>
    void setColumn(ColumnNumber column, T && columnData);

    //! Returns the number of rows of the table.
    RowNumber rowCount() const;

    //! Returns the number of columns of the table.
    ColumnNumber columnCount() const;

    friend std::ostream &operator<<(std::ostream &os, const Textable &table);

private:
    using Row = std::vector<std::string>;
    using Table = std::vector<Row>;

    Table m_table;
};

#endif // !__TEXTABLE_H__

