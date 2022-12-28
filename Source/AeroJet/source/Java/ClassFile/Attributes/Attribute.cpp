/*
 * Attribute.cpp
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

#include "Java/ClassFile/Attributes/Attribute.hpp"
#include "Java/ClassFile/Utils/ConstantPoolEntryUtils.hpp"
#include "Exceptions/IncorrectAttributeTypeException.hpp"

namespace AeroJet::Java::ClassFile
{
    Attribute::Attribute(const ConstantPool& constantPool, const AttributeInfo& attributeInfo, std::string_view requiredAttributeName)
    {
        const u2 nameIndex = attributeInfo.attributeNameIndex();
        const std::string attributeName = Utils::ConstantPoolInfoUtf8::asString(constantPool[nameIndex]);

        if (attributeName != requiredAttributeName)
        {
            throw Exceptions::IncorrectAttributeTypeException(requiredAttributeName, attributeName);
        }

        m_attributeNameIndex = nameIndex;
        m_attributeLength = attributeInfo.size();

        const std::vector<u1>& info = attributeInfo.info();
        std::move(info.begin(), info.end(), std::ostream_iterator<u1>(m_infoDataStream));
    }

    u2 Attribute::attributeNameIndex() const
    {
        return m_attributeNameIndex;
    }

    u4 Attribute::attributeLength() const
    {
        return m_attributeLength;
    }
}
