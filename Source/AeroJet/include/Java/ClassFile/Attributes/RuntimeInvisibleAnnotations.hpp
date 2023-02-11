/*
 * RuntimeInvisibleAnnotations.hpp
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

#include "Java/ClassFile/Attributes/Annotation/Annotation.hpp"
#include "Java/ClassFile/Attributes/Attribute.hpp"

#include <vector>

namespace AeroJet::Java::ClassFile
{
    /**
     * The RuntimeInvisibleAnnotations attribute is a variable-length attribute in the attributes table of a ClassFile,
     * field_info, or method_info structure (§4.1, §4.5, §4.6). The RuntimeInvisibleAnnotations
     * attribute records run-time invisible annotations on the declaration of the corresponding class, method, or field.
     *
     * There may be at most one RuntimeInvisibleAnnotations attribute in the attributes table of a ClassFile,
     * field_info, or method_info structure.
     *
     * The RuntimeInvisibleAnnotations attribute is similar to the RuntimeVisibleAnnotations attribute (§4.7.16),
     * except that the annotations represented by a RuntimeInvisibleAnnotations attribute must not be made available
     * for return by reflective APIs, unless the Java Virtual Machine has been instructed to retain these annotations
     * via some implementation-specific mechanism such as a command line flag. In the absence of such instructions,
     * the Java Virtual Machine ignores this attribute.
     */
    class RuntimeInvisibleAnnotations : public Attribute
    {
      public:
        static constexpr auto RUNTIME_INVISIBLE_ANNOTATIONS_ATTRIBUTE_NAME = "RuntimeInvisibleAnnotations";

        RuntimeInvisibleAnnotations(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        /**
         * The value of the num_annotations item gives the number of run-time visible annotations represented by
         * the structure.
         */
        [[nodiscard]] u2 numAnnotation() const;

        /**
         * Each entry in the annotations table represents a single run-time visible annotation on a declaration.
         */
        [[nodiscard]] const std::vector<Annotation>& annotations() const;

      private:
        std::vector<Annotation> m_annotations;
    };
} // namespace AeroJet::Java::ClassFile