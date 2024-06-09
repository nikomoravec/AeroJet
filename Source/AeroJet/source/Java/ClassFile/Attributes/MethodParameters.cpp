/*
 * MethodParameters.cpp
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

#include "Java/ClassFile/Attributes/MethodParameters.hpp"

#include "Stream/StandardStreamWrapper.hpp"

namespace AeroJet::Java::ClassFile
{

    MethodParameters::MethodParameter::MethodParameter(u2 nameIndex,
                                                       MethodParameters::MethodParameterAccessFlags accessFlags) :
        m_nameIndex(nameIndex),
        m_accessFlags(accessFlags)
    {
    }

    u2 MethodParameters::MethodParameter::nameIndex() const
    {
        return m_nameIndex;
    }

    MethodParameters::MethodParameterAccessFlags MethodParameters::MethodParameter::accessFlags() const
    {
        return m_accessFlags;
    }

    MethodParameters::MethodParameters(const ConstantPool& constantPool, const AttributeInfo& attributeInfo) :
        Attribute(constantPool, attributeInfo, METHOD_PARAMETERS_ATTRIBUTE_NAME)
    {
        const u1 parametersCount = Stream::Reader::read<u1>(m_infoDataStream, Stream::ByteOrder::INVERSE);

        m_methodParameters.reserve(parametersCount);
        for(u1 parameterIndex = 0; parameterIndex < parametersCount; parameterIndex++)
        {
            const u2 nameIndex = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            const MethodParameterAccessFlags accessFlags = static_cast<MethodParameterAccessFlags>(
                Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE));

            m_methodParameters.emplace_back(nameIndex, accessFlags);
        }
    }

    u1 MethodParameters::parametersCount() const
    {
        return m_methodParameters.size();
    }

    const std::vector<MethodParameters::MethodParameter>& MethodParameters::methodParameters() const
    {
        return m_methodParameters;
    }
} // namespace AeroJet::Java::ClassFile
