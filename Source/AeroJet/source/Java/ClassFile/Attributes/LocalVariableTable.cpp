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

#include "Java/ClassFile/Attributes/LocalVariableTable.hpp"
#include "Exceptions/IncorrectAttributeTypeException.hpp"
#include "Stream/Reader.hpp"

namespace AeroJet::Java::ClassFile
{
   LocalVariableTable::LocalVariableTableEntry::LocalVariableTableEntry(u2 startPc, u2 length, u2 nameIndex, u2 descriptorIndex, u2 index) :
           m_startPc(startPc),
           m_length(length),
           m_nameIndex(nameIndex),
           m_descriptorIndex(descriptorIndex),
           m_index(index)
   {
   }

    u2 LocalVariableTable::LocalVariableTableEntry::startPc() const
    {
        return m_startPc;
    }

    u2 LocalVariableTable::LocalVariableTableEntry::length() const
    {
        return m_length;
    }

    u2 LocalVariableTable::LocalVariableTableEntry::nameIndex() const
    {
        return m_nameIndex;
    }

    u2 LocalVariableTable::LocalVariableTableEntry::descriptorIndex() const
    {
        return m_descriptorIndex;
    }

    u2 LocalVariableTable::LocalVariableTableEntry::index() const
    {
        return m_index;
    }

    LocalVariableTable::LocalVariableTable(const ConstantPool& constantPool, const AttributeInfo& attributeInfo) : Attribute(constantPool, attributeInfo, LOCAL_VARIABLE_TABLE_ATTRIBUTE_NAME)
    {
        const u2 localVariableTableLength = Stream::Reader::read<u2>(m_infoDataStream);
        m_localVariableTable.reserve(localVariableTableLength);
        for (int32_t localVariableTableIndex = 0; localVariableTableIndex < localVariableTableLength; localVariableTableIndex++)
        {
            const u2 entryStartPc = Stream::Reader::read<u2>(m_infoDataStream);
            const u2 entryLength = Stream::Reader::read<u2>(m_infoDataStream);
            const u2 entryNameIndex = Stream::Reader::read<u2>(m_infoDataStream);
            const u2 entryDescriptorIndex = Stream::Reader::read<u2>(m_infoDataStream);
            const u2 entryIndex = Stream::Reader::read<u2>(m_infoDataStream);

            m_localVariableTable.emplace_back(entryStartPc, entryLength, entryNameIndex, entryDescriptorIndex, entryIndex);
        }
    }

    const std::vector<LocalVariableTable::LocalVariableTableEntry>& LocalVariableTable::localVariableTable() const
    {
        return m_localVariableTable;
    }
}
