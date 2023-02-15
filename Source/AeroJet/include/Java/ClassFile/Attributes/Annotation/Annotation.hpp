/*
 * Annotation.hpp
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

#include "Java/ClassFile/Attributes/Annotation/ElementValue.hpp"
#include "Java/ClassFile/Attributes/Annotation/ElementValuePair.hpp"
#include "Types.hpp"

#include <vector>

namespace AeroJet::Java::ClassFile
{
    class Annotation
    {
      public:
        Annotation(u2 typeIndex, const std::vector<ElementValuePair>& elementValuePairs);

        /**
         * The value of the type_index item must be a valid index into the constant_pool table.
         * The constant_pool entry at that index must be a CONSTANT_Utf8_info structure (§4.4.7)
         * representing a field descriptor (§4.3.2). The field descriptor denotes the type of the annotation
         * represented by this annotation structure.
         */
        [[nodiscard]] u2 typeIndex() const;

        /**
         * The value of the num_element_value_pairs item gives the number of element-value pairs of the annotation
         * represented by this annotation structure.
         */
        [[nodiscard]] u2 numElementValuePairs() const;

        /**
         * Each value of the element_value_pairs table represents a single element-value pair in the annotation
         * represented by this annotation structure.
         */
        [[nodiscard]] const std::vector<ElementValuePair>& elementValuePairs() const;

      private:
        u2                            m_typeIndex;
        std::vector<ElementValuePair> m_elementValuePairs;
    };
} // namespace AeroJet::Java::ClassFile