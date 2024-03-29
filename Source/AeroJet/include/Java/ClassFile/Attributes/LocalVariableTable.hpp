/*
 * LineNumberTable.hpp
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

#include "Java/ClassFile/Attributes/Attribute.hpp"
#include "Java/ClassFile/Attributes/AttributeInfo.hpp"
#include "Java/ClassFile/ConstantPool.hpp"
#include "Types.hpp"

#include <vector>

namespace AeroJet::Java::ClassFile
{
    class LocalVariableTable : public Attribute
    {
      public:
        static constexpr auto LOCAL_VARIABLE_TABLE_ATTRIBUTE_NAME = "LocalVariableTable";

        class LocalVariableTableEntry
        {
          public:
            LocalVariableTableEntry(u2 startPc, u2 length, u2 nameIndex, u2 descriptorIndex, u2 index);

            [[nodiscard]] u2 startPc() const;

            [[nodiscard]] u2 length() const;

            [[nodiscard]] u2 nameIndex() const;

            [[nodiscard]] u2 descriptorIndex() const;

            [[nodiscard]] u2 index() const;

          protected:
            u2 m_startPc;
            u2 m_length;
            u2 m_nameIndex;
            u2 m_descriptorIndex;
            u2 m_index;
        };

        LocalVariableTable(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        [[nodiscard]] const std::vector<LocalVariableTableEntry>& localVariableTable() const;

      protected:
        std::vector<LocalVariableTableEntry> m_localVariableTable;
    };
} // namespace AeroJet::Java::ClassFile
