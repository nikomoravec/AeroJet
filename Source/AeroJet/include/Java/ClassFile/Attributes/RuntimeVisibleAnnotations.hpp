/*
 * RuntimeVisibleAnnotations.hpp
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

#include "Java/ClassFile/Attributes/Annotation/Annotation.hpp"
#include "Java/ClassFile/Attributes/Attribute.hpp"

#include <vector>

namespace AeroJet::Java::ClassFile
{
    class RuntimeVisibleAnnotations : public Attribute
    {
      public:
        static constexpr auto RUNTIME_VISIBLE_ANNOTATIONS_ATTRIBUTE_NAME = "RuntimeVisibleAnnotations";

        RuntimeVisibleAnnotations(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

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