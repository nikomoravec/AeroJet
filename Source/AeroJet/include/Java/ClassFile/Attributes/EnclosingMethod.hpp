/*
 * EnclosingMethod.hpp
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

#pragma once

#include "Java/ClassFile/Attributes/Attribute.hpp"
#include "Java/ClassFile/Attributes/AttributeInfo.hpp"
#include "Java/ClassFile/ConstantPool.hpp"
#include "Types.hpp"

namespace AeroJet::Java::ClassFile
{
    /**
     * The EnclosingMethod attribute is a fixed-length attribute in the attributes table of a ClassFile structure (§4.1).
     * A class must have an EnclosingMethod attribute if and only if it represents a local class or an anonymous
     * class (JLS §14.3, JLS §15.9.5).
     *
     * There may be at most one EnclosingMethod attribute in the attributes table of a ClassFile structure
     */
    class EnclosingMethod : public Attribute
    {
      public:
        static constexpr auto ENCLOSING_METHOD_ATTRIBUTE_NAME = "EnclosingMethod";

        EnclosingMethod(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        /**
         * The value of the class_index item must be a valid index into the constant_pool table.
         * The constant_pool entry at that index must be a CONSTANT_Class_info structure (§4.4.1) representing the
         * innermost class that encloses the declaration of the current class.
         */
        [[nodiscard]] u2 classIndex() const;

        /**
         * If the current class is not immediately enclosed by a method or constructor,
         * then the value of the method_index item must be zero.
         *
         * In particular, method_index must be zero if the current class was immediately enclosed in source code
         * by an instance initializer, static initializer, instance variable initializer, or class variable initializer.
         * (The first two concern both local classes and anonymous classes, while the last two concern anonymous classes
         * declared on the right hand side of a field assignment.)
         *
         * Otherwise, the value of the method_index item must be a valid index into the constant_pool table.
         * The constant_pool entry at that index must be a CONSTANT_NameAndType_info structure (§4.4.6) representing
         * the name and type of a method in the class referenced by the class_index attribute above.
         *
         * It is the responsibility of a Java compiler to ensure that the method identified via the method_index is
         * indeed the closest lexically enclosing method of the class that contains this EnclosingMethod attribute.
         */
        [[nodiscard]] u2 methodIndex() const;

      protected:
        u2 m_classIndex;
        u2 m_methodIndex;
    };
} // namespace AeroJet::Java::ClassFile
