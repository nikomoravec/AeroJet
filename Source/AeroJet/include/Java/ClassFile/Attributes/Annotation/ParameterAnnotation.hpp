/*
 * ParameterAnnotation.hpp
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
#include "Stream/JavaClassStream.hpp"

#include <vector>

namespace AeroJet::Java::ClassFile
{
    class ParameterAnnotation
    {
      public:
        explicit ParameterAnnotation(const std::vector<Annotation>& annotations);

        /**
         * The value of the num_annotations item indicates the number of run-time visible annotations on the declaration
         * of the formal parameter corresponding to the parameter_annotations entry.
         */
        [[nodiscard]] u2 numAnnotations() const;

        /**
         * Each entry in the annotations table represents a single run-time visible annotation on the declaration of the
         * formal parameter corresponding to the parameter_annotations entry.
         * The annotation structure is specified in §4.7.16.
         */
        [[nodiscard]] const std::vector<Annotation>& annotations() const;

        template<typename T>
        static ParameterAnnotation read(Stream::JavaClassStream<T>& stream)
        {
            const u2 numAnnotations = stream.template read<u2>();
            std::vector<Annotation> annotations = stream.template readSome<Annotation>(numAnnotations);

            return AeroJet::Java::ClassFile::ParameterAnnotation{ annotations };
        }

      private:
        std::vector<Annotation> m_annotations;
    };
} // namespace AeroJet::Java::ClassFile