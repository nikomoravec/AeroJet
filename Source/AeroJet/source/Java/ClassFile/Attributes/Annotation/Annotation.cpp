/*
 * Annotation.cpp
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

#include "Java/ClassFile/Attributes/Annotation/Annotation.hpp"

#include "Stream/Reader.hpp"

namespace AeroJet::Java::ClassFile
{

    Annotation::ElementValuePair::ElementValuePair(u2 elementNameIndex, const ElementValue& value) :
        m_elementNameIndex(elementNameIndex), m_value(value)
    {
    }

    u2 Annotation::ElementValuePair::elementNameIndex() const
    {
        return m_elementNameIndex;
    }

    const ElementValue& Annotation::ElementValuePair::value() const
    {
        return m_value;
    }

    Annotation::Annotation(u2 typeIndex, const std::vector<ElementValuePair>& elementValuePairs) :
        m_typeIndex(typeIndex), m_elementValuePairs(elementValuePairs)
    {
    }

    u2 Annotation::typeIndex() const
    {
        return m_typeIndex;
    }

    u2 Annotation::numElementValuePairs() const
    {
        return m_elementValuePairs.size();
    }

    const std::vector<Annotation::ElementValuePair>& Annotation::elementValuePairs() const
    {
        return m_elementValuePairs;
    }
} // namespace AeroJet::Java::ClassFile

template<>
AeroJet::Java::ClassFile::Annotation AeroJet::Stream::Reader::read(std::istream& stream, ByteOrder byteOrder)
{
    const u2 typeIndex = AeroJet::Stream::Reader::read<u2>(stream, byteOrder);

    const u2 numElementValuePairs = AeroJet::Stream::Reader::read<u2>(stream, byteOrder);
    std::vector<AeroJet::Java::ClassFile::Annotation::ElementValuePair> elementValues;
    elementValues.reserve(numElementValuePairs);
    for(u2 elementValueIndex = 0; elementValueIndex < numElementValuePairs; elementValueIndex++)
    {
        const u2                            elementNameIndex = AeroJet::Stream::Reader::read<u2>(stream, byteOrder);
        const Java::ClassFile::ElementValue elementValue =
            AeroJet::Stream::Reader::read<Java::ClassFile::ElementValue>(stream, byteOrder);

        AeroJet::Java::ClassFile::Annotation::ElementValuePair elementValuePair{ elementNameIndex, elementValue };
        elementValues.emplace_back(elementValuePair);
    }

    return AeroJet::Java::ClassFile::Annotation{ typeIndex, elementValues };
}
