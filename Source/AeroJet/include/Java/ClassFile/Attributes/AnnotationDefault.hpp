/*
 * AnnotationDefault.hpp
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
#include "Java/ClassFile/Attributes/Annotation/ElementValue.hpp"

namespace AeroJet::Java::ClassFile
{
    /**
     * The AnnotationDefault attribute is a variable-length attribute in the attributes table of certain method_info
     * structures (§4.6), namely those representing elements of annotation types (JLS §9.6.1).
     * The AnnotationDefault attribute records the default value (JLS §9.6.2) for the element represented by the
     * method_info structure. The Java Virtual Machine must make this default value available so it can be applied by
     * appropriate reflective APIs.
     *
     * There may be at most one AnnotationDefault attribute in the attributes table of a method_info structure
     * which represents an element of an annotation type.
     */
    class AnnotationDefault : public Attribute
    {
      public:
        static constexpr auto ANNOTATION_DEFAULT_ATTRIBUTE_NAME = "AnnotationDefault";

        AnnotationDefault(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        /**
         * The default_value item represents the default value of the annotation type element represented by the
         * method_info structure enclosing this AnnotationDefault attribute.
         */
        [[nodiscard]] const ElementValue& defaultValue() const;

      private:
        ElementValue m_defaultValue;
    };
} // namespace AeroJet::Java::ClassFile