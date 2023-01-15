/*
 * Code.cpp
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

#include "Java/ClassFile/Attributes/Code.hpp"
#include "Stream/Reader.hpp"

namespace AeroJet::Java::ClassFile
{
    Code::ExceptionTableEntry::ExceptionTableEntry(u2 startPc, u2 endPc, u2 handlePc, u2 catchType) :
        m_startPc(startPc),
        m_endPc(endPc),
        m_handlerPc(handlePc),
        m_catchType(catchType)
    {
    }

    u2 Code::ExceptionTableEntry::startPc() const
    {
        return m_startPc;
    }

    u2 Code::ExceptionTableEntry::endPc() const
    {
        return m_endPc;
    }

    u2 Code::ExceptionTableEntry::handlerPc() const
    {
        return m_handlerPc;
    }

    u2 Code::ExceptionTableEntry::catchType() const
    {
        return m_catchType;
    }

    Code::Code(const ConstantPool& constantPool, const AttributeInfo& attributeInfo) : Attribute(constantPool, attributeInfo, CODE_ATTRIBUTE_NAME)
    {
        m_maxStack = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
        m_maxLocals = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);

        const u4 codeLength = Stream::Reader::read<u4>(m_infoDataStream, Stream::ByteOrder::INVERSE);

        const u4 currentPos = static_cast<u4>(m_infoDataStream.tellg());
        const u4 endPos = currentPos + codeLength;
        while (m_infoDataStream.tellg() != endPos)
        {
            m_code.emplace_back(Stream::Reader::read<ByteCode::Instruction>(m_infoDataStream, Stream::ByteOrder::INVERSE));
        }

        const u2 exceptionTableLength = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
        m_exceptionTable.reserve(exceptionTableLength);
        for (int32_t exceptionTableIndex = 0; exceptionTableIndex < exceptionTableLength; exceptionTableIndex++)
        {
            const u2 startPc = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            const u2 endPc = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            const u2 handlePc = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            const u2 catchType = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);

            m_exceptionTable.emplace_back(startPc, endPc, handlePc, catchType);
        }

        const u2 attributesCount = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);

        m_attributes.reserve(attributesCount);
        for (int32_t attributeIndex = 0; attributeIndex < attributesCount; attributeIndex++)
        {
            m_attributes.emplace_back(Stream::Reader::read<AttributeInfo>(m_infoDataStream, Stream::ByteOrder::INVERSE));
        }
    }

    u2 Code::maxStack() const
    {
        return m_maxStack;
    }

    u2 Code::maxLocals() const
    {
        return m_maxLocals;
    }

    const std::vector<ByteCode::Instruction>& Code::code() const
    {
        return m_code;
    }

    const std::vector<Code::ExceptionTableEntry>& Code::exceptionTable() const
    {
        return m_exceptionTable;
    }

    const std::vector<AttributeInfo>& Code::attributes() const
    {
        return m_attributes;
    }
}
