/*
 * LocalVariableTypeTable.hpp
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
#include "Java/ClassFile/Attributes/AttributeInfo.hpp"
#include "Java/ClassFile/ConstantPool.hpp"
#include "Types.hpp"

#include <vector>

namespace AeroJet::Java::ClassFile
{
    /**
     * The LocalVariableTypeTable attribute is an optional variable-length attribute in the attributes table of a Code
     * attribute (§4.7.3). It may be used by debuggers to determine the value of a given local variable during
     * the execution of a method.
     *
     * If multiple LocalVariableTypeTable attributes are present in the attributes table of a given Code attribute,
     * then they may appear in any order.
     *
     * There may be no more than one LocalVariableTypeTable attribute per local variable in the attributes table
     * of a Code attribute
     */
    class LocalVariableTypeTable : public Attribute
    {
      public:
        static constexpr auto LOCAL_VARIABLE_TYPES_TABLE_ATTRIBUTE_NAME = "LocalVariableTypeTable";

        class LocalVariableTypeTableEntry
        {
          public:
            LocalVariableTypeTableEntry(u2 startPc, u2 length, u2 nameIndex, u2 signatureIndex, u2 index);

            [[nodiscard]] u2 startPc() const;

            [[nodiscard]] u2 length() const;

            [[nodiscard]] u2 nameIndex() const;

            [[nodiscard]] u2 signatureIndex() const;

            [[nodiscard]] u2 index() const;

          protected:
            /**
             * start_pc, length
             * The given local variable must have a value at indices into the code array in the interval
             * [start_pc, start_pc + length), that is, between start_pc inclusive and start_pc + length exclusive.
             * The value of start_pc must be a valid index into the code array of this Code attribute and
             * must be the index of the opcode of an instruction.
             * The value of start_pc + length must either be a valid index into the code array of this Code attribute
             * and be the index of the opcode of an instruction, or it must be the first index beyond the
             * end of that code array.
             */
            u2 m_startPc;
            u2 m_length;

            /**
             * The value of the name_index item must be a valid index into the constant_pool table.
             * The constant_pool entry at that index must contain a CONSTANT_Utf8_info structure (§4.4.7)
             * representing a valid unqualified name denoting a local variable (§4.2.2).
             */
            u2 m_nameIndex;

            /**
             * The value of the signature_index item must be a valid index into the constant_pool table.
             * The constant_pool entry at that index must contain a CONSTANT_Utf8_info structure (§4.4.7) representing
             * a field signature which encodes the type of a local variable in the source program (§4.7.9.1).
             */
            u2 m_signatureIndex;

            /**
             * The given local variable must be at index in the local variable array of the current frame.
             * If the local variable at index is of type double or long, it occupies both index and index + 1.
             */
            u2 m_index;
        };

        LocalVariableTypeTable(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        [[nodiscard]] const std::vector<LocalVariableTypeTableEntry>& localVariableTypeTable() const;

      protected:
        /**
         * Each entry in the local_variable_type_table array indicates a range of code array offsets within
         * which a local variable has a value. It also indicates the index into the local variable array of the current
         * frame at which that local variable can be found.
         */
        std::vector<LocalVariableTypeTableEntry> m_localVariableTypeTable;
    };
} // namespace AeroJet::Java::ClassFile
