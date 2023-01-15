/*
 * AttributeInfo.cpp
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

#include "Java/ClassFile/Attributes/AttributeInfo.hpp"
#include "Stream/Reader.hpp"
#include "Types.hpp"

namespace AeroJet::Java::ClassFile
{
    AttributeInfo::AttributeInfo(u2 attributeIndex, std::vector<u1> info) :
        m_attributeNameIndex(attributeIndex),
        m_info(std::move(info))
    {
    }

    u2 AttributeInfo::attributeNameIndex() const
    {
        return m_attributeNameIndex;
    }

    u2 AttributeInfo::size() const
    {
        return m_info.size();
    }

    const std::vector<u1>& AttributeInfo::info() const
    {
        return m_info;
    }
}


template<>
AeroJet::Java::ClassFile::AttributeInfo AeroJet::Stream::Reader::read(std::istream& stream, ByteOrder byteOrder)
{
    const AeroJet::u2 attributeNameIndex = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);
    const AeroJet::u4 attributeInfoSize = AeroJet::Stream::Reader::read<AeroJet::u4>(stream, byteOrder);

    std::vector<AeroJet::u1> attributeInfo;
    attributeInfo.reserve(attributeInfoSize);
    for (i4 attributeInfoIndex = 0; attributeInfoIndex < attributeInfoSize; attributeInfoIndex++)
    {
        const AeroJet::u1 byte = AeroJet::Stream::Reader::read<AeroJet::u1>(stream, byteOrder);
        attributeInfo.emplace_back(byte);
    }

    return {attributeNameIndex, attributeInfo};
}
