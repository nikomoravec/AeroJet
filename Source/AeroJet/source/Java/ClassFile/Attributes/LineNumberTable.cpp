/*
 * LineNumberTable.cpp
 *
 * Copyright © 2023 AeroJet Developers. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "Java/ClassFile/Attributes/LineNumberTable.hpp"

#include "Stream/Reader.hpp"

namespace AeroJet::Java::ClassFile
{
    LineNumberTable::LineNumberTableEntry::LineNumberTableEntry(u2 startPc, u2 lineNumber) :
        m_startPc(startPc), m_lineNumber(lineNumber)
    {
    }

    u2 LineNumberTable::LineNumberTableEntry::startPc() const
    {
        return m_startPc;
    }

    u2 LineNumberTable::LineNumberTableEntry::lineNumber() const
    {
        return m_lineNumber;
    }

    LineNumberTable::LineNumberTable(const ConstantPool& constantPool, const AttributeInfo& attributeInfo) :
        Attribute(constantPool, attributeInfo, LINE_NUMBER_TABLE_ATTRIBUTE_NAME)
    {
        const u2 lineNumberTableLength = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
        m_lineNumberTable.reserve(lineNumberTableLength);
        for(i4 lineNumberTableEntryIndex = 0; lineNumberTableEntryIndex < lineNumberTableLength;
            lineNumberTableEntryIndex++)
        {
            const u2 startPc    = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            const u2 lineNumber = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            m_lineNumberTable.emplace_back(startPc, lineNumber);
        }
    }

    const std::vector<LineNumberTable::LineNumberTableEntry>& LineNumberTable::lineNumberTable() const
    {
        return m_lineNumberTable;
    }
} // namespace AeroJet::Java::ClassFile