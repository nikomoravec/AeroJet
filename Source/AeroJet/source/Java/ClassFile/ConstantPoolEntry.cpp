/*
 * ConstantPoolEntry.cpp
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

#include "Java/ClassFile/ConstantPoolEntry.hpp"

#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"
#include "Stream/StandardStreamWrapper.hpp"

namespace AeroJet::Java::ClassFile
{
    ConstantPoolEntry::ConstantPoolEntry(const ConstantPoolInfoTag tag, DataStream& data) :
        m_tag(tag), m_data(data)
    {
    }

    ConstantPoolInfoTag ConstantPoolEntry::tag() const
    {
        return m_tag;
    }

    ConstantPoolInfoUtf8::ConstantPoolInfoUtf8(const std::vector<u1>& bytes) :
        m_string(bytes.begin(), bytes.end()) {}

    u2 ConstantPoolInfoUtf8::length() const
    {
        return m_string.size();
    }

    std::string ConstantPoolInfoUtf8::asString() const
    {
        return m_string;
    }

    std::vector<u1> ConstantPoolInfoUtf8::bytes() const
    {
        return std::vector<u1>{ m_string.begin(), m_string.end() };
    }

    ConstantPoolInfoInteger::ConstantPoolInfoInteger(u4 bytes) :
        m_bytes(bytes) {}

    u4 ConstantPoolInfoInteger::bytes() const
    {
        return m_bytes;
    }

    ConstantPoolInfoLong::ConstantPoolInfoLong(u4 highBytes, u4 lowBytes) :
        m_highBytes(highBytes), m_lowBytes(lowBytes)
    {
    }

    u4 ConstantPoolInfoLong::highBytes() const
    {
        return m_highBytes;
    }

    u4 ConstantPoolInfoLong::lowBytes() const
    {
        return m_lowBytes;
    }

    ConstantPoolInfoClass::ConstantPoolInfoClass(u2 nameIndex) :
        m_nameIndex(nameIndex) {}

    u2 ConstantPoolInfoClass::nameIndex() const
    {
        return m_nameIndex;
    }

    ConstantPoolInfoString::ConstantPoolInfoString(u2 stringIndex) :
        m_stringIndex(stringIndex) {}

    u2 ConstantPoolInfoString::stringIndex() const
    {
        return m_stringIndex;
    }

    ConstantPoolInfoFieldRef::ConstantPoolInfoFieldRef(u2 classIndex, u2 nameAndTypeIndex) :
        m_classIndex(classIndex), m_nameAndTypeIndex(nameAndTypeIndex)
    {
    }

    u2 ConstantPoolInfoFieldRef::classIndex() const
    {
        return m_classIndex;
    }

    u2 ConstantPoolInfoFieldRef::nameAndTypeIndex() const
    {
        return m_nameAndTypeIndex;
    }

    ConstantPoolInfoNameAndType::ConstantPoolInfoNameAndType(u2 nameIndex, u2 descriptorIndex) :
        m_nameIndex(nameIndex), m_descriptorIndex(descriptorIndex)
    {
    }

    u2 ConstantPoolInfoNameAndType::nameIndex() const
    {
        return m_nameIndex;
    }

    u2 ConstantPoolInfoNameAndType::descriptorIndex() const
    {
        return m_descriptorIndex;
    }

    ConstantPoolInfoMethodHandle::ConstantPoolInfoMethodHandle(
        ConstantPoolInfoMethodHandle::ReferenceKind referenceKind,
        u2 referenceIndex) :
        m_referenceKind(referenceKind),
        m_referenceIndex(referenceIndex)
    {
    }

    ConstantPoolInfoMethodHandle::ReferenceKind ConstantPoolInfoMethodHandle::referenceKind() const
    {
        return m_referenceKind;
    }

    u2 ConstantPoolInfoMethodHandle::referenceIndex() const
    {
        return m_referenceIndex;
    }

    ConstantPoolInfoMethodType::ConstantPoolInfoMethodType(u2 descriptorIndex) :
        m_descriptorIndex(descriptorIndex) {}

    u2 ConstantPoolInfoMethodType::descriptorIndex() const
    {
        return m_descriptorIndex;
    }

    ConstantPoolInfoInvokeDynamic::ConstantPoolInfoInvokeDynamic(u2 bootstrapMethodAttributeIndex,
                                                                 u2 nameAndTypeIndex) :
        m_bootstrapMethodAttributeIndex(bootstrapMethodAttributeIndex),
        m_nameAndTypeIndex(nameAndTypeIndex)
    {
    }

    u2 ConstantPoolInfoInvokeDynamic::bootstrapMethodAttributeIndex() const
    {
        return m_bootstrapMethodAttributeIndex;
    }

    u2 ConstantPoolInfoInvokeDynamic::nameAndTypeIndex() const
    {
        return m_nameAndTypeIndex;
    }

    template<>
    ConstantPoolInfoUtf8 ConstantPoolEntry::as() const
    {
        return ConstantPoolInfoUtf8{ data() };
    }

    template<>
    ConstantPoolInfoInteger ConstantPoolEntry::as() const
    {
        constexpr u2 BYTES_OFFSET = 0;
        return ConstantPoolInfoInteger{ *(u4*)(&m_data[BYTES_OFFSET]) };
    }

    template<>
    ConstantPoolInfoLong ConstantPoolEntry::as() const
    {
        constexpr u2 HIGH_BYTES_OFFSET = 0;
        constexpr u2 LOW_BYTES_OFFSET = 4;

        return ConstantPoolInfoLong{ *(u4*)(&m_data[HIGH_BYTES_OFFSET]), *(u4*)(&m_data[LOW_BYTES_OFFSET]) };
    }

    template<>
    ConstantPoolInfoClass ConstantPoolEntry::as() const
    {
        constexpr u2 NAME_INDEX_OFFSET = 0;
        return ConstantPoolInfoClass{ *(u2*)(&m_data[NAME_INDEX_OFFSET]) };
    }

    template<>
    ConstantPoolInfoString ConstantPoolEntry::as() const
    {
        constexpr u2 STRING_INDEX_OFFSET = 0;
        return ConstantPoolInfoString{ *(u2*)(&m_data[STRING_INDEX_OFFSET]) };
    }

    template<>
    ConstantPoolInfoFieldRef ConstantPoolEntry::as() const
    {
        constexpr u2 CLASS_INDEX_OFFSET = 0;
        constexpr u2 NAME_AND_TYPE_INDEX_OFFSET = 2;

        return ConstantPoolInfoFieldRef{ *(u2*)(&m_data[CLASS_INDEX_OFFSET]),
                                         *(u2*)(&m_data[NAME_AND_TYPE_INDEX_OFFSET]) };
    }

    template<>
    ConstantPoolInfoNameAndType ConstantPoolEntry::as() const
    {
        constexpr u2 NAME_INDEX_OFFSET = 0;
        constexpr u2 DESCRIPTOR_INDEX_OFFSET = 2;

        return ConstantPoolInfoNameAndType{ *(u2*)(&m_data[NAME_INDEX_OFFSET]),
                                            *(u2*)(&m_data[DESCRIPTOR_INDEX_OFFSET]) };
    }

    template<>
    ConstantPoolInfoMethodHandle ConstantPoolEntry::as() const
    {
        constexpr u2 REFERENCE_KIND_OFFSET = 0;
        constexpr u2 REFERENCE_INDEX_OFFSET = 1;

        return ConstantPoolInfoMethodHandle{ static_cast<ConstantPoolInfoMethodHandle::ReferenceKind>(
                                                 *(u2*)(&m_data[REFERENCE_KIND_OFFSET])),
                                             *(u2*)(&m_data[REFERENCE_INDEX_OFFSET]) };
    }

    template<>
    ConstantPoolInfoMethodType ConstantPoolEntry::as() const
    {
        constexpr u2 DESCRIPTOR_INDEX_OFFSET = 0;
        return ConstantPoolInfoMethodType{ *(u2*)(&m_data[DESCRIPTOR_INDEX_OFFSET]) };
    }

    template<>
    ConstantPoolInfoInvokeDynamic ConstantPoolEntry::as() const
    {
        constexpr u2 BOOTSTRAP_METHOD_ATTRIBUTE_INDEX_OFFSET = 0;
        constexpr u2 NAME_AND_TYPE_INDEX_OFFSET = 2;

        return ConstantPoolInfoInvokeDynamic{ *(u2*)(&m_data[BOOTSTRAP_METHOD_ATTRIBUTE_INDEX_OFFSET]),
                                              *(u2*)(&m_data[NAME_AND_TYPE_INDEX_OFFSET]) };
    }
} // namespace AeroJet::Java::ClassFile
