/*
 * ConstantPoolEntry.cpp
 *
 * Copyright © 2022 AeroJet Developers. All Rights Reserved.
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
#include "Stream/Reader.hpp"
#include "Stream/StreamUtils.hpp"
#include "Stream/Writer.hpp"

namespace
{
    inline void checkDataSize(const AeroJet::Java::ClassFile::ConstantPoolEntry& entry, AeroJet::u8 requiredSize)
    {

        const std::size_t dataSize = entry.data().size();
        if (dataSize < requiredSize)
        {
            throw AeroJet::Exceptions::RuntimeException(fmt::format("Data size '{}' is less than required '{}'", dataSize, requiredSize));
        }
    }

    inline void verifyTag(const AeroJet::Java::ClassFile::ConstantPoolEntry& entry, AeroJet::Java::ClassFile::ConstantPoolInfoTag requiredTag)
    {
        const AeroJet::Java::ClassFile::ConstantPoolInfoTag tag = entry.tag();
        if (tag != requiredTag)
        {
            throw AeroJet::Exceptions::RuntimeException(
                    fmt::format("entry tag '{}' is not equal to required '{}'",
                                static_cast<AeroJet::u1>(tag),
                                static_cast<AeroJet::u1>(requiredTag))
            );
        }
    }
}

namespace AeroJet::Java::ClassFile
{
    constexpr u2 CONSTANT_POOL_INFO_CLASS_DATA_SIZE = 2;
    constexpr u2 CONSTANT_POOL_INFO_FIELD_REF_SIZE = 4;
    constexpr u2 CONSTANT_POOL_INFO_STRING_SIZE = 2;
    constexpr u2 CONSTANT_POOL_INFO_INTEGER_SIZE = 4;
    constexpr u2 CONSTANT_POOL_INFO_LONG_SIZE = 8;
    constexpr u2 CONSTANT_POOL_INFO_NAME_AND_TYPE_SIZE = 4;
    constexpr u2 CONSTANT_POOL_INFO_METHOD_HANDLE_SIZE = 3;
    constexpr u2 CONSTANT_POOL_INFO_METHOD_TYPE_SIZE = 2;
    constexpr u2 CONSTANT_POOL_INFO_INVOKE_DYNAMIC_SIZE = 2;

    ConstantPoolEntry::ConstantPoolEntry(const ConstantPoolInfoTag tag, const std::vector<u1>& data) : m_tag(tag), m_data(data)
    {
    }

    ConstantPoolInfoTag ConstantPoolEntry::tag() const
    {
        return m_tag;
    }

    const std::vector<u1>& ConstantPoolEntry::data() const
    {
        return m_data;
    }

    u2 ConstantPoolInfoUtf8::length(const ConstantPoolEntry& entry)
    {
        verifyTag(entry, ConstantPoolInfoTag::UTF_8);
        return entry.data().size();
    }

    u2 ConstantPoolInfoClass::nameIndex(const ConstantPoolEntry& entry)
    {
        constexpr u2 NAME_INDEX_OFFSET = 0;

        verifyTag(entry, ConstantPoolInfoTag::CLASS);
        checkDataSize(entry, CONSTANT_POOL_INFO_CLASS_DATA_SIZE);
        return *(u2*)(&entry.data()[NAME_INDEX_OFFSET]);
    }

    u2 ConstantPoolInfoFieldRef::classIndex(const ConstantPoolEntry& entry)
    {
        constexpr u2 CLASS_INDEX_OFFSET = 0;

        verifyTag(entry, ConstantPoolInfoTag::FIELD_REF);
        checkDataSize(entry, CONSTANT_POOL_INFO_FIELD_REF_SIZE);
        return *(u2*)(&entry.data()[CLASS_INDEX_OFFSET]);
    }

    u2 ConstantPoolInfoFieldRef::nameAndTypeIndex(const ConstantPoolEntry& entry)
    {
        constexpr u2 NAME_AND_TYPE_INDEX_OFFSET = 2;

        verifyTag(entry, ConstantPoolInfoTag::FIELD_REF);
        checkDataSize(entry, CONSTANT_POOL_INFO_FIELD_REF_SIZE);
        return *(u2*)(&entry.data()[NAME_AND_TYPE_INDEX_OFFSET]);
    }

    u2 ConstantPoolInfoString::stringIndex(const ConstantPoolEntry& entry)
    {
        constexpr u2 STRING_INDEX_OFFSET = 0;

        verifyTag(entry, ConstantPoolInfoTag::STRING);
        checkDataSize(entry, CONSTANT_POOL_INFO_STRING_SIZE);
        return *(u2*)(&entry.data()[STRING_INDEX_OFFSET]);
    }

    u4 ConstantPoolInfoInteger::bytes(const ConstantPoolEntry& entry)
    {
        constexpr u2 BYTES_OFFSET = 0;

        verifyTag(entry, ConstantPoolInfoTag::INTEGER);
        checkDataSize(entry, CONSTANT_POOL_INFO_INTEGER_SIZE);
        return *(u4*)(&entry.data()[BYTES_OFFSET]);
    }

    u4 ConstantPoolInfoLong::highBytes(const ConstantPoolEntry& entry)
    {
        constexpr u2 HIGH_BYTES_OFFSET = 0;

        verifyTag(entry, ConstantPoolInfoTag::LONG);
        checkDataSize(entry, CONSTANT_POOL_INFO_LONG_SIZE);
        return *(u4*)(&entry.data()[HIGH_BYTES_OFFSET]);
    }

    u4 ConstantPoolInfoLong::lowBytes(const ConstantPoolEntry& entry)
    {
        constexpr u2 LOW_BYTES_OFFSET = 4;

        verifyTag(entry, ConstantPoolInfoTag::LONG);
        checkDataSize(entry, CONSTANT_POOL_INFO_LONG_SIZE);
        return *(u4*)(&entry.data()[LOW_BYTES_OFFSET]);
    }

    u2 ConstantPoolInfoNameAndType::nameIndex(const ConstantPoolEntry& entry)
    {
        constexpr u2 NAME_INDEX_OFFSET = 0;

        verifyTag(entry, ConstantPoolInfoTag::NAME_AND_TYPE);
        checkDataSize(entry, CONSTANT_POOL_INFO_NAME_AND_TYPE_SIZE);
        return *(u2*)(&entry.data()[NAME_INDEX_OFFSET]);
    }

    u2 ConstantPoolInfoNameAndType::descriptorIndex(const ConstantPoolEntry& entry)
    {
        constexpr u2 DESCRIPTOR_INDEX_OFFSET = 2;

        verifyTag(entry, ConstantPoolInfoTag::NAME_AND_TYPE);
        checkDataSize(entry, CONSTANT_POOL_INFO_NAME_AND_TYPE_SIZE);
        return *(u2*)(&entry.data()[DESCRIPTOR_INDEX_OFFSET]);
    }

    ConstantPoolInfoMethodHandle::ReferenceKind ConstantPoolInfoMethodHandle::referenceKind(const ConstantPoolEntry& entry)
    {
        constexpr u2 REFERENCE_KIND_OFFSET = 0;

        verifyTag(entry, ConstantPoolInfoTag::METHOD_HANDLE);
        checkDataSize(entry, CONSTANT_POOL_INFO_METHOD_HANDLE_SIZE);
        return static_cast<ConstantPoolInfoMethodHandle::ReferenceKind>(*(u1*)(&entry.data()[REFERENCE_KIND_OFFSET]));
    }

    u2 ConstantPoolInfoMethodHandle::referenceIndex(const ConstantPoolEntry& entry)
    {
        constexpr u2 REFERENCE_INDEX_OFFSET = 1;

        verifyTag(entry, ConstantPoolInfoTag::METHOD_HANDLE);
        checkDataSize(entry, CONSTANT_POOL_INFO_METHOD_HANDLE_SIZE);
        return *(u2*)(&entry.data()[REFERENCE_INDEX_OFFSET]);
    }

    u2 ConstantPoolInfoMethodType::descriptorIndex(const ConstantPoolEntry& entry)
    {
        constexpr u2 DESCRIPTOR_INDEX_OFFSET = 0;

        verifyTag(entry, ConstantPoolInfoTag::METHOD_TYPE);
        checkDataSize(entry, CONSTANT_POOL_INFO_METHOD_TYPE_SIZE);
        return *(u2*)(&entry.data()[DESCRIPTOR_INDEX_OFFSET]);
    }

    u2 ConstantPoolInfoInvokeDynamic::bootstrapMethodAttributeIndex(const ConstantPoolEntry& entry)
    {
        constexpr u2 BOOTSTRAP_METHOD_ATTRIBUTE_INDEX_OFFSET = 0;

        verifyTag(entry, ConstantPoolInfoTag::INVOKE_DYNAMIC);
        checkDataSize(entry, CONSTANT_POOL_INFO_INVOKE_DYNAMIC_SIZE);
        return *(u2*)(&entry.data()[BOOTSTRAP_METHOD_ATTRIBUTE_INDEX_OFFSET]);
    }

    u2 ConstantPoolInfoInvokeDynamic::nameAndTypeIndex(const ConstantPoolEntry& entry)
    {
        constexpr u2 NAME_AND_TYPE_INDEX_OFFSET = 2;

        verifyTag(entry, ConstantPoolInfoTag::INVOKE_DYNAMIC);
        checkDataSize(entry, CONSTANT_POOL_INFO_INVOKE_DYNAMIC_SIZE);
        return *(u2*)(&entry.data()[NAME_AND_TYPE_INDEX_OFFSET]);
    }
}

template<>
AeroJet::Java::ClassFile::ConstantPoolEntry AeroJet::Stream::Reader::read(std::istream& stream)
{
    std::stringstream dataStream;

    const AeroJet::Java::ClassFile::ConstantPoolInfoTag tag = static_cast<AeroJet::Java::ClassFile::ConstantPoolInfoTag>(AeroJet::Stream::Reader::read<u1>(stream));
    switch (tag)
    {
        case Java::ClassFile::ConstantPoolInfoTag::UTF_8:
        {
            const AeroJet::u2 dataSize = AeroJet::Stream::Reader::read<AeroJet::u2>(stream);
            for (AeroJet::u2 byteIndex = 0; byteIndex < dataSize; byteIndex++)
            {
                AeroJet::u1 byte = AeroJet::Stream::Reader::read<AeroJet::u1>(stream);
                AeroJet::Stream::Writer::write(dataStream, byte);
            }

            break;
        }
        case Java::ClassFile::ConstantPoolInfoTag::INTEGER:
        case Java::ClassFile::ConstantPoolInfoTag::FLOAT:
        {
            const AeroJet::u4 value = AeroJet::Stream::Reader::read<AeroJet::u4>(stream);
            AeroJet::Stream::Writer::write(dataStream, value);

            break;
        }
        case Java::ClassFile::ConstantPoolInfoTag::LONG:
        case Java::ClassFile::ConstantPoolInfoTag::DOUBLE:
        {
            const AeroJet::u4 highBytes = AeroJet::Stream::Reader::read<AeroJet::u4>(stream);
            const AeroJet::u4 lowBytes = AeroJet::Stream::Reader::read<AeroJet::u4>(stream);

            AeroJet::Stream::Writer::write(dataStream, highBytes);
            AeroJet::Stream::Writer::write(dataStream, lowBytes);

            break;
        }
        case Java::ClassFile::ConstantPoolInfoTag::CLASS:
        case Java::ClassFile::ConstantPoolInfoTag::STRING:
        case Java::ClassFile::ConstantPoolInfoTag::METHOD_TYPE:
        {
            const AeroJet::u2 index = AeroJet::Stream::Reader::read<AeroJet::u2>(stream);
            AeroJet::Stream::Writer::write(dataStream, index);

            break;
        }
        case Java::ClassFile::ConstantPoolInfoTag::FIELD_REF:
        case Java::ClassFile::ConstantPoolInfoTag::METHOD_REF:
        case Java::ClassFile::ConstantPoolInfoTag::INTERFACE_METHOD_REF:
        case Java::ClassFile::ConstantPoolInfoTag::NAME_AND_TYPE:
        case Java::ClassFile::ConstantPoolInfoTag::INVOKE_DYNAMIC:
        {
            const AeroJet::u2 index1 = AeroJet::Stream::Reader::read<AeroJet::u2>(stream);
            const AeroJet::u2 index2 = AeroJet::Stream::Reader::read<AeroJet::u2>(stream);

            AeroJet::Stream::Writer::write(dataStream, index1);
            AeroJet::Stream::Writer::write(dataStream, index2);

            break;
        }
        case Java::ClassFile::ConstantPoolInfoTag::METHOD_HANDLE:
        {
            const AeroJet::u1 referenceKind = AeroJet::Stream::Reader::read<AeroJet::u1>(stream);
            const AeroJet::u2 referenceIndex = AeroJet::Stream::Reader::read<AeroJet::u2>(stream);

            AeroJet::Stream::Writer::write(dataStream, referenceKind);
            AeroJet::Stream::Writer::write(dataStream, referenceIndex);

            break;
        }
        default:
            throw AeroJet::Exceptions::RuntimeException(
                    fmt::format(
                            "Unexpected Constant Pool Entry Tag {:#04x} at position {:#08x}!",
                            static_cast<u2>(tag),
                            stream.tellg()
                    ));
    }

    std::vector<AeroJet::u1> dataBytes = AeroJet::Stream::StreamUtils::streamToBytes(dataStream);
    return {tag, dataBytes};
}
