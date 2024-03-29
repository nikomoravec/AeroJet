/*
 * RuntimeVisibleTyperAnnotations.hpp
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

#include "Java/ClassFile/Attributes/Annotation/TypeAnnotation.hpp"
#include "Java/ClassFile/Attributes/Attribute.hpp"

#include <vector>

namespace AeroJet::Java::ClassFile
{
    /**
     * The RuntimeVisibleTypeAnnotations attribute is an variable-length attribute in the attributes table of a
     * ClassFile, field_info, or method_info structure, or Code attribute (§4.1, §4.5, §4.6, §4.7.3).
     * The RuntimeVisibleTypeAnnotations attribute records run-time visible annotations on types used in the
     * declaration of the corresponding class, field, or method, or in an expression in the corresponding method body.
     * The RuntimeVisibleTypeAnnotations attribute also records run-time visible annotations on type parameter
     * declarations of generic classes, interfaces, methods, and constructors. The Java Virtual Machine must make
     * these annotations available so they can be returned by the appropriate reflective APIs.
     *
     * There may be at most one RuntimeVisibleTypeAnnotations attribute in the attributes table of a ClassFile,
     * field_info, or method_info structure, or Code attribute.
     *
     * An attributes table contains a RuntimeVisibleTypeAnnotations attribute only if types are annotated in kinds of
     * declaration or expression that correspond to the parent structure or attribute of the attributes table.                                                                                                                                                                        For example, all annotations on types in the implements clause of a class declaration are recorded in the RuntimeVisibleTypeAnnotations attribute of the class's ClassFile structure. Meanwhile, all annotations on the type in a field declaration are recorded in the RuntimeVisibleTypeAnnotations attribute of the field's field_info structure.
     */
    class RuntimeVisibleTyperAnnotations : public Attribute
    {
      public:
        static constexpr auto RUNTIME_VISIBLE_TYPE_ANNOTATIONS_ATTRIBUTE_NAME = "RuntimeVisibleTyperAnnotations";

        RuntimeVisibleTyperAnnotations(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        /**
         * The value of the num_annotations item gives the number of run-time visible type annotations represented
         * by the structure.
         */
        [[nodiscard]] u2 numAnnotations() const;

        /**
         * Each entry in the annotations table represents a single run-time visible annotation on a type used in a
         * declaration or expression.
         */
        [[nodiscard]] const std::vector<TypeAnnotation>& annotations() const;

      private:
        std::vector<TypeAnnotation> m_annotations;
    };
} // namespace AeroJet::Java::ClassFile
