/*
 * ConstantPoolEntry.hpp
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

#pragma once

#include "Stream/JavaClassStream.hpp"
#include "Stream/StandardStreamWrapper.hpp"
#include "Types.hpp"

#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace AeroJet::Java::ClassFile
{
    enum class ConstantPoolInfoTag : u1
    {
        UTF_8 = 1,
        INTEGER = 3,
        FLOAT = 4,
        LONG = 5,
        DOUBLE = 6,
        CLASS = 7,
        STRING = 8,
        FIELD_REF = 9,
        METHOD_REF = 10,
        INTERFACE_METHOD_REF = 11,
        NAME_AND_TYPE = 12,
        METHOD_HANDLE = 15,
        METHOD_TYPE = 16,
        INVOKE_DYNAMIC = 18
    };

    class ConstantPoolInfoUtf8
    {
      public:
        explicit ConstantPoolInfoUtf8(const std::vector<u1>& bytes);

        [[nodiscard]] u2 length() const;

        [[nodiscard]] std::vector<u1> bytes() const;

        [[nodiscard]] std::string asString() const;

      private:
        std::string m_string;
    };

    class ConstantPoolInfoInteger
    {
      public:
        explicit ConstantPoolInfoInteger(u4 bytes);

        [[nodiscard]] u4 bytes() const;

      private:
        u4 m_bytes;
    };

    using ConstantPoolInfoFloat = ConstantPoolInfoInteger;

    class ConstantPoolInfoLong
    {
      public:
        ConstantPoolInfoLong(u4 highBytes, u4 lowBytes);

        [[nodiscard]] u4 highBytes() const;

        [[nodiscard]] u4 lowBytes() const;

      private:
        u4 m_highBytes;
        u4 m_lowBytes;
    };

    using ConstantPoolInfoDouble = ConstantPoolInfoLong;

    class ConstantPoolInfoClass
    {
      public:
        explicit ConstantPoolInfoClass(u2 nameIndex);

        [[nodiscard]] u2 nameIndex() const;

      private:
        u2 m_nameIndex;
    };

    class ConstantPoolInfoString
    {
      public:
        explicit ConstantPoolInfoString(u2 stringIndex);

        [[nodiscard]] u2 stringIndex() const;

      private:
        u2 m_stringIndex;
    };

    class ConstantPoolInfoFieldRef
    {
      public:
        ConstantPoolInfoFieldRef(u2 classIndex, u2 nameAndTypeIndex);

        [[nodiscard]] u2 classIndex() const;

        [[nodiscard]] u2 nameAndTypeIndex() const;

      private:
        u2 m_classIndex;
        u2 m_nameAndTypeIndex;
    };

    using ConstantPoolInfoMethodRef = ConstantPoolInfoFieldRef;
    using ConstantPoolInfoInterfaceMethodRef = ConstantPoolInfoFieldRef;

    class ConstantPoolInfoNameAndType
    {
      public:
        ConstantPoolInfoNameAndType(u2 nameIndex, u2 descriptorIndex);

        [[nodiscard]] u2 nameIndex() const;

        [[nodiscard]] u2 descriptorIndex() const;

      private:
        u2 m_nameIndex;
        u2 m_descriptorIndex;
    };

    class ConstantPoolInfoMethodHandle
    {
      public:
        enum class ReferenceKind : u1
        {
            REF_getField = 1,
            REF_getStatic = 2,
            REF_putField = 3,
            REF_putStatic = 4,
            REF_invokeVirtual = 5,
            REF_invokeStatic = 6,
            REF_invokeSpecial = 7,
            REF_newInvokeSpecial = 8,
            REF_invokeInterface = 9
        };

        ConstantPoolInfoMethodHandle(ReferenceKind referenceKind, u2 referenceIndex);

        [[nodiscard]] ReferenceKind referenceKind() const;

        [[nodiscard]] u2 referenceIndex() const;

      private:
        ReferenceKind m_referenceKind;
        u2 m_referenceIndex;
    };

    class ConstantPoolInfoMethodType
    {
      public:
        explicit ConstantPoolInfoMethodType(u2 descriptorIndex);

        [[nodiscard]] u2 descriptorIndex() const;

      private:
        u2 m_descriptorIndex;
    };

    class ConstantPoolInfoInvokeDynamic
    {
      public:
        ConstantPoolInfoInvokeDynamic(u2 bootstrapMethodAttributeIndex, u2 nameAndTypeIndex);

        [[nodiscard]] u2 bootstrapMethodAttributeIndex() const;

        [[nodiscard]] u2 nameAndTypeIndex() const;

      private:
        u2 m_bootstrapMethodAttributeIndex;
        u2 m_nameAndTypeIndex;
    };

    class ConstantPoolEntry
    {
      public:
        ConstantPoolEntry(ConstantPoolInfoTag tag, const std::vector<u1>& data);

        [[nodiscard]] ConstantPoolInfoTag tag() const;

        [[nodiscard]] const std::vector<u1>& data() const;

        template<typename T>
        static ConstantPoolEntry read(Stream::JavaClassStream<T>& stream)
        {
            Stream::StandardStreamWrapper<std::stringstream> dataStream;

            const AeroJet::Java::ClassFile::ConstantPoolInfoTag tag = static_cast<AeroJet::Java::ClassFile::ConstantPoolInfoTag>(stream.template read<u1>());
            switch(tag)
            {
                case Java::ClassFile::ConstantPoolInfoTag::UTF_8:
                {
                    const AeroJet::u2 dataSize = stream.template read<u2>();
                    for(AeroJet::u2 byteIndex = 0; byteIndex < dataSize; byteIndex++)
                    {
                        AeroJet::u1 byte = stream.template read<u1>();
                        dataStream.write<u1>(byte);
                    }

                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::INTEGER:
                case Java::ClassFile::ConstantPoolInfoTag::FLOAT:
                {
                    const AeroJet::u4 value = stream.template read<u4>();
                    dataStream.write<u4>(value);

                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::LONG:
                case Java::ClassFile::ConstantPoolInfoTag::DOUBLE:
                {
                    const AeroJet::u4 highBytes = stream.template read<u4>();
                    const AeroJet::u4 lowBytes = stream.template read<u4>();

                    dataStream.write<u4>(highBytes);
                    dataStream.write<u4>(lowBytes);

                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::CLASS:
                case Java::ClassFile::ConstantPoolInfoTag::STRING:
                case Java::ClassFile::ConstantPoolInfoTag::METHOD_TYPE:
                {
                    const AeroJet::u2 index = stream.template read<u2>();
                    dataStream.write<u2>(index);

                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::FIELD_REF:
                case Java::ClassFile::ConstantPoolInfoTag::METHOD_REF:
                case Java::ClassFile::ConstantPoolInfoTag::INTERFACE_METHOD_REF:
                case Java::ClassFile::ConstantPoolInfoTag::NAME_AND_TYPE:
                case Java::ClassFile::ConstantPoolInfoTag::INVOKE_DYNAMIC:
                {
                    const AeroJet::u2 index1 = stream.template read<u2>();
                    const AeroJet::u2 index2 = stream.template read<u2>();

                    dataStream.write<u2>(index1);
                    dataStream.write<u2>(index2);

                    break;
                }
                case Java::ClassFile::ConstantPoolInfoTag::METHOD_HANDLE:
                {
                    const AeroJet::u1 referenceKind = stream.template read<u1>();
                    const AeroJet::u2 referenceIndex = stream.template read<u2>();

                    dataStream.write<u1>(referenceKind);
                    dataStream.write<u2>(referenceKind);

                    break;
                }
                default:
                    throw AeroJet::Exceptions::RuntimeException(fmt::format("Unexpected Constant Pool Entry Tag {:#04x} at position {:#08x}!", static_cast<u2>(tag), static_cast<std::size_t>(stream.tellg())));
            }

            return { tag, dataStream };
        }

        /*
         * FIXME: this is kinda unsafe because of no m_tag validation inside specialized realizations
         * Author: Nikita Miroshnichenko (nikita.miroshnichenko@yahoo.com)
         */
        template<typename T>
        T as() const;

      protected:
        ConstantPoolInfoTag m_tag;
        Stream::StandardStreamWrapper<std::stringstream> m_data;
    };
} // namespace AeroJet::Java::ClassFile
