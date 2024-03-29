/*
 * ElementValuePair.hpp
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

#include "Java/ClassFile/Attributes/Annotation/ElementValue.hpp"

#pragma once

namespace AeroJet::Java::ClassFile
{
    class ElementValuePair
    {
      public:
        ElementValuePair(u2 elementNameIndex, const ElementValue& value);

        /**
         * The value of the element_name_index item must be a valid index into the constant_pool table.
         * The constant_pool entry at that index must be a CONSTANT_Utf8_info structure (§4.4.7).
         * The constant_pool entry denotes the name of the element of the element-value pair represented
         * by this element_value_pairs entry.
         */
        [[nodiscard]] u2 elementNameIndex() const;

        /**
         * The value of the value item represents the value of the element-value pair represented by this
         * element_value_pairs entry.
         */
        [[nodiscard]] const ElementValue& value() const;

      private:
        u2 m_elementNameIndex;
        ElementValue m_value;
    };
} // namespace AeroJet::Java::ClassFile
