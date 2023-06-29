/*
 * SourceDebugExtension.cpp
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

#include "Java/ClassFile/Attributes/SourceDebugExtension.hpp"

#include "Stream/Reader.hpp"

namespace AeroJet::Java::ClassFile
{

    SourceDebugExtension::SourceDebugExtension(const ConstantPool& constantPool, const AttributeInfo& attributeInfo) :
        Attribute(constantPool, attributeInfo, SOURCE_DEBUG_EXTENSION_ATTRIBUTE_NAME)
    {
        m_debugExtension.reserve(m_attributeLength);
        for(size_t index = 0; index < m_attributeLength; index++)
        {
            const u1 byte = Stream::Reader::read<u1>(m_infoDataStream, Stream::ByteOrder::INVERSE);
            m_debugExtension.emplace_back(byte);
        }
    }

    const std::vector<u1>& SourceDebugExtension::debugExtension() const
    {
        return m_debugExtension;
    }
} // namespace AeroJet::Java::ClassFile
