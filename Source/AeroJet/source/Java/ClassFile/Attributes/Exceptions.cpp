/*
 * Exceptions.hpp
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

#include "Java/ClassFile/Attributes/Exceptions.hpp"

#include "Stream/StandardStreamWrapper.hpp"

namespace AeroJet::Java::ClassFile
{
    Exceptions::Exceptions(const ConstantPool& constantPool, const AttributeInfo& attributeInfo) :
        Attribute(constantPool, attributeInfo, EXCEPTIONS_ATTRIBUTE_NAME)
    {
        const u2 numberOfExceptions = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
        m_exceptionIndexTable.reserve(numberOfExceptions);

        for(size_t exceptionIndex = 0; exceptionIndex < numberOfExceptions; exceptionIndex++)
        {
            m_exceptionIndexTable.emplace_back(Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE));
        }
    }

    u2 Exceptions::numberOfExceptions()
    {
        return m_exceptionIndexTable.size();
    }

    const std::vector<u2>& Exceptions::exceptionIndexTable()
    {
        return m_exceptionIndexTable;
    }
} // namespace AeroJet::Java::ClassFile
