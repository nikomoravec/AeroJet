/*
 * AttributeInfo.hpp
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

#include "Types.hpp"

#include <vector>

namespace AeroJet::Java::ClassFile
{
    /**
     * 4.7 Attributes
     *
     * Attributes are used in the ClassFile (§4.1), field_info (§4.5), method_info (§4.6), and Code_attribute (§4.7.3)
     * structures of the class file format. All attributes have the following general format:
     *
     * attribute_info {
     *      u2 attribute_name_index;
     *      u4 attribute_length;
     *      u1 info[attribute_length];
     * }
     *
     * @see https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7
     */

    class AttributeInfo
    {
      public:
        AttributeInfo(u2 attributeIndex, std::vector<u1> info);

        [[nodiscard]] u2 attributeNameIndex() const;

        [[nodiscard]] u2 size() const;

        [[nodiscard]] const std::vector<u1>& info() const;

      protected:
        u2 m_attributeNameIndex;
        std::vector<u1> m_info;
    };
} // namespace AeroJet::Java::ClassFile
