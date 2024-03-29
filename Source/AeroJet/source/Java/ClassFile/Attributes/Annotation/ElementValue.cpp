/*
 * ElementValue.cpp
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

#include "Java/ClassFile/Attributes/Annotation/ElementValue.hpp"

#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"
#include "Java/ClassFile/Attributes/Annotation/Annotation.hpp"
#include "Stream/Reader.hpp"

#include <utility>

namespace AeroJet::Java::ClassFile
{
    ElementValue::EnumConstValue::EnumConstValue(u2 typeNameIndex, u2 constNameIndex) :
        m_typeNameIndex(typeNameIndex), m_constNameIndex(constNameIndex)
    {
    }

    u2 ElementValue::EnumConstValue::typeNameIndex() const
    {
        return m_typeNameIndex;
    }

    u2 ElementValue::EnumConstValue::constNameIndex() const
    {
        return m_constNameIndex;
    }

    ElementValue::ArrayValue::ArrayValue(const std::vector<ElementValue>& values) :
        m_values(values) {}

    u2 ElementValue::ArrayValue::numValues() const
    {
        return m_values.size();
    }

    const std::vector<ElementValue>& ElementValue::ArrayValue::values() const
    {
        return m_values;
    }

    ElementValue::ElementValue(ElementValue::Tag tag, ElementValue::Value value) :
        m_tag(tag), m_value(std::move(value))
    {
    }

    ElementValue::Tag ElementValue::tag() const
    {
        return m_tag;
    }

    const ElementValue::Value& ElementValue::value() const
    {
        return m_value;
    }
} // namespace AeroJet::Java::ClassFile

template<>
AeroJet::Java::ClassFile::ElementValue AeroJet::Stream::Reader::read(std::istream& stream, ByteOrder byteOrder)
{
    const Java::ClassFile::ElementValue::Tag tag =
        static_cast<Java::ClassFile::ElementValue::Tag>(AeroJet::Stream::Reader::read<u1>(stream, byteOrder));

    switch(tag)
    {
        case Java::ClassFile::ElementValue::Tag::BYTE:
        case Java::ClassFile::ElementValue::Tag::CHAR:
        case Java::ClassFile::ElementValue::Tag::DOUBLE:
        case Java::ClassFile::ElementValue::Tag::FLOAT:
        case Java::ClassFile::ElementValue::Tag::INT:
        case Java::ClassFile::ElementValue::Tag::LONG:
        case Java::ClassFile::ElementValue::Tag::SHORT:
        case Java::ClassFile::ElementValue::Tag::BOOLEAN:
        case Java::ClassFile::ElementValue::Tag::STRING:
        {
            const u2 constValueIndex = AeroJet::Stream::Reader::read<u2>(stream, byteOrder);
            return AeroJet::Java::ClassFile::ElementValue{ tag, AeroJet::u2{ constValueIndex } };
        }
        case Java::ClassFile::ElementValue::Tag::ENUM_TYPE:
        {
            const u2 typeNameIndex = AeroJet::Stream::Reader::read<u2>(stream, byteOrder);
            const u2 constValueIndex = AeroJet::Stream::Reader::read<u2>(stream, byteOrder);

            return AeroJet::Java::ClassFile::ElementValue{
                tag,
                Java::ClassFile::ElementValue::EnumConstValue{ typeNameIndex, constValueIndex }
            };
        }
        case Java::ClassFile::ElementValue::Tag::CLASS:
        {
            const u2 classInfoIndex = AeroJet::Stream::Reader::read<u2>(stream, byteOrder);
            return AeroJet::Java::ClassFile::ElementValue{ tag, AeroJet::u2{ classInfoIndex } };
        }
        case Java::ClassFile::ElementValue::Tag::ANNOTATION_TYPE:
        {
            const AeroJet::Java::ClassFile::Annotation annotation =
                AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::Annotation>(stream, byteOrder);

            return AeroJet::Java::ClassFile::ElementValue{ tag,
                                                           std::make_shared<AeroJet::Java::ClassFile::Annotation>(
                                                               annotation) };
        }
        case Java::ClassFile::ElementValue::Tag::ARRAY_TYPE:
        {
            const u2 numValues = AeroJet::Stream::Reader::read<u2>(stream, byteOrder);

            std::vector<AeroJet::Java::ClassFile::ElementValue> values;
            values.reserve(numValues);
            for(u2 valueIndex = 0; valueIndex < numValues; valueIndex++)
            {
                values.emplace_back(
                    AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::ElementValue>(stream, byteOrder));
            }

            return AeroJet::Java::ClassFile::ElementValue{ tag,
                                                           AeroJet::Java::ClassFile::ElementValue::ArrayValue{
                                                               values } };
        }
        default:
            throw Exceptions::RuntimeException(fmt::format("Unknown ElementValue tag: {}", static_cast<u1>(tag)));
    }
}
