/*
 * FieldInfo.cpp
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

#include "Java/ClassFile/FieldInfo.hpp"

#include "Java/ClassFile/Attributes/AttributeInfo.hpp"
#include "Stream/Reader.hpp"

namespace AeroJet::Java::ClassFile
{

    /*
     * 4.5 Fields
     *
     * Each field is described by a field_info structure.
     * No two fields in one class file may have the same name and descriptor (§4.3.2).
     * The format of this structure is
     *
     * field_info {
     *	u2 access_flags;
     *	u2 name_index;
     *	u2 descriptor_index;
     *	u2 attributes_count;
     *	attribute_info attributes[attributes_count];
     * }
     */

    FieldInfo::FieldInfo(u2 accessFlags,
                         u2 nameIndex,
                         u2 descriptorIndex,
                         const std::vector<AttributeInfo>& attributes) :
        m_accessFlags(accessFlags),
        m_nameIndex(nameIndex), m_descriptorIndex(descriptorIndex), m_attributes(attributes)
    {
    }

    FieldInfo::AccessFlags FieldInfo::accessFlags() const
    {
        return static_cast<AccessFlags>(m_accessFlags);
    }

    u2 FieldInfo::nameIndex() const
    {
        return m_nameIndex;
    }

    u2 FieldInfo::descriptorIndex() const
    {
        return m_descriptorIndex;
    }

    const std::vector<AttributeInfo>& FieldInfo::attributes() const
    {
        return m_attributes;
    }
} // namespace AeroJet::Java::ClassFile

template<>
AeroJet::Java::ClassFile::FieldInfo AeroJet::Stream::Reader::read(std::istream& stream, ByteOrder byteOrder)
{
    const AeroJet::u2 accessFlags = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);
    const AeroJet::u2 nameIndex = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);
    const AeroJet::u2 descriptorIndex = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);
    const AeroJet::u2 attributesCount = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);

    std::vector<AeroJet::Java::ClassFile::AttributeInfo> attributes;
    attributes.reserve(attributesCount);
    for(AeroJet::u4 attributeIndex = 0; attributeIndex < attributesCount; attributeIndex++)
    {
        attributes.emplace_back(
            AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::AttributeInfo>(stream, byteOrder));
    }

    return { accessFlags, nameIndex, descriptorIndex, attributes };
}
