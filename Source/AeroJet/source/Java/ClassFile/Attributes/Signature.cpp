/*
 * Signature.cpp
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

#include "Java/ClassFile/Attributes/Signature.hpp"

#include "Exceptions/RuntimeException.hpp"
#include "Stream/Reader.hpp"

namespace AeroJet::Java::ClassFile
{
    Signature::Signature(const ConstantPool &constantPool, const AttributeInfo &attributeInfo) :
        Attribute(constantPool, attributeInfo, SIGNATURE_ATTRIBUTE_NAME)
    {
        /**
         * The value of the attribute_length item of a Signature_attribute structure must be two.
         */
        constexpr u2 SIGNATURE_ATTRIBUTE_LENGTH = 2;
        if(m_attributeLength != SIGNATURE_ATTRIBUTE_LENGTH)
        {
            throw Exceptions::RuntimeException(
                "The value of the attributeLength item of a Signature attribute structure must be two");
        }

        m_signatureIndex = Stream::Reader::read<u2>(m_infoDataStream, Stream::ByteOrder::INVERSE);
    }
} // namespace AeroJet::Java::ClassFile
