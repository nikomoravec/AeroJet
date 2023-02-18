/*
 * RuntimeInvisibleTyperAnnotations.hpp
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

#include "Java/ClassFile/Attributes/Annotation/TypeAnnotation.hpp"
#include "Java/ClassFile/Attributes/Attribute.hpp"

#include <vector>

namespace AeroJet::Java::ClassFile
{
    /**
     * The RuntimeInvisibleTypeAnnotations attribute is an variable-length attribute in the attributes table of a
     * ClassFile, field_info, or method_info structure, or Code attribute (§4.1, §4.5, §4.6, §4.7.3).
     * The RuntimeInvisibleTypeAnnotations attribute records run-time invisible annotations on types used in the
     * corresponding declaration of a class, field, or method, or in an expression in the corresponding method body.
     * The RuntimeInvisibleTypeAnnotations attribute also records annotations on type parameter declarations of
     * generic classes, interfaces, methods, and constructors.
     *
     * There may be at most one RuntimeInvisibleTypeAnnotations attribute in the attributes table of a ClassFile,
     * field_info, or method_info structure, or Code attribute.
     *
     * An attributes table contains a RuntimeInvisibleTypeAnnotations attribute only if types are annotated in kinds
     * of declaration or expression that correspond to the parent structure or attribute of the attributes table.
     */
    class RuntimeInvisibleTyperAnnotations : public Attribute
    {
      public:
        static constexpr auto RUNTIME_INVISIBLE_TYPE_ANNOTATIONS_ATTRIBUTE_NAME = "RuntimeInvisibleTyperAnnotations";

        RuntimeInvisibleTyperAnnotations(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        /**
         * The value of the num_annotations item gives the number of run-time invisible type annotations represented
         * by the structure.
         */
        [[nodiscard]] u2 numAnnotations() const;

        /**
         * Each entry in the annotations table represents a single run-time invisible annotation on a type used in a
         * declaration or expression. The type_annotation structure is specified in §4.7.20.
         */
        [[nodiscard]] const std::vector<TypeAnnotation>& annotations() const;

      private:
        std::vector<TypeAnnotation> m_annotations;
    };
} // namespace AeroJet::Java::ClassFile