/*
 * LocalVariableTypeTable.cpp
 *
 * Copyright © 2024 AeroJet Developers. All Rights Reserved.
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

#include "Java/ClassFile/Attributes/LocalVariableTypeTable.hpp"

#include "Stream/StandardStreamWrapper.hpp"

namespace AeroJet::Java::ClassFile
{
    LocalVariableTypeTable::LocalVariableTypeTableEntry::LocalVariableTypeTableEntry(u2 startPc,
                                                                                     u2 length,
                                                                                     u2 nameIndex,
                                                                                     u2 signatureIndex,
                                                                                     u2 index) :
        m_startPc(startPc),
        m_length(length), m_nameIndex(nameIndex), m_signatureIndex(signatureIndex), m_index(index)
    {
    }

    u2 LocalVariableTypeTable::LocalVariableTypeTableEntry::startPc() const
    {
        return m_startPc;
    }

    u2 LocalVariableTypeTable::LocalVariableTypeTableEntry::length() const
    {
        return m_length;
    }

    u2 LocalVariableTypeTable::LocalVariableTypeTableEntry::nameIndex() const
    {
        return m_nameIndex;
    }

    u2 LocalVariableTypeTable::LocalVariableTypeTableEntry::signatureIndex() const
    {
        return m_signatureIndex;
    }

    u2 LocalVariableTypeTable::LocalVariableTypeTableEntry::index() const
    {
        return m_index;
    }

    LocalVariableTypeTable::LocalVariableTypeTable(const ConstantPool& constantPool,
                                                   const AttributeInfo& attributeInfo) :
        Attribute(constantPool, attributeInfo, LOCAL_VARIABLE_TYPES_TABLE_ATTRIBUTE_NAME)
    {
        const u2 localVariableTypeTableLength = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
        m_localVariableTypeTable.reserve(localVariableTypeTableLength);

        for(int32_t localVariableTypeTableIndex = 0; localVariableTypeTableIndex < localVariableTypeTableLength;
            localVariableTypeTableIndex++)
        {
            const u2 startPc = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            const u2 length = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            const u2 nameIndex = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            const u2 signatureIndex = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            const u2 index = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);

            m_localVariableTypeTable.emplace_back(startPc, length, nameIndex, signatureIndex, index);
        }
    }

    const std::vector<LocalVariableTypeTable::LocalVariableTypeTableEntry>& LocalVariableTypeTable::localVariableTypeTable() const
    {
        return m_localVariableTypeTable;
    }
} // namespace AeroJet::Java::ClassFile
