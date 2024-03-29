/*
 * Signature.hpp
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

#include "Java/ClassFile/Attributes/Attribute.hpp"
#include "Java/ClassFile/Attributes/AttributeInfo.hpp"
#include "Java/ClassFile/ConstantPool.hpp"
#include "Types.hpp"

namespace AeroJet::Java::ClassFile
{
    /**
     * The Signature attribute is a fixed-length attribute in the attributes table of a ClassFile, field_info,
     * or method_info structure (§4.1, §4.5, §4.6). A Signature attribute records a signature (§4.7.9.1) for a class,
     * interface, constructor, method, or field whose declaration in the Java programming language uses type variables
     * or parameterized types. See The Java Language Specification, Java SE 8 Edition for details about these types.
     */
    class Signature : public Attribute
    {
      public:
        static constexpr auto SIGNATURE_ATTRIBUTE_NAME = "Signature";

        Signature(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        [[nodiscard]] u2 signatureIndex() const;

      protected:
        /**
         * The value of the signature_index item must be a valid index into the constant_pool table.
         * The constant_pool entry at that index must be a CONSTANT_Utf8_info structure (§4.4.7) representing a class
         * signature if this Signature attribute is an attribute of a ClassFile structure; a method signature
         * if this Signature attribute is an attribute of a method_info structure; or a field signature otherwise.
         */
        u2 m_signatureIndex;
    };
} // namespace AeroJet::Java::ClassFile