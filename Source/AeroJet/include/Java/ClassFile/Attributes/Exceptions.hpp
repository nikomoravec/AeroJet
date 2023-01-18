/*
 * Exceptions.hpp
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

namespace AeroJet::Java::ClassFile
{
    /**
     * 4.7.5. The Exceptions Attribute
     * The Exceptions attribute is a variable-length attribute in the attributes table of a method_info structure (§4.6). The Exceptions attribute indicates which checked exceptions a method may throw.
     * There may be at most one Exceptions attribute in the attributes table of a method_info structure.
     * The Exceptions attribute has the following format:
     * Exceptions_attribute {
     *   u2 attribute_name_index;
     *   u4 attribute_length;
     *   u2 number_of_exceptions;
     *   u2 exception_index_table[number_of_exceptions];
     * }
     *
     * @see https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.5
     */
    class Exceptions : public Attribute
    {
    public:
        static constexpr auto EXCEPTIONS_ATTRIBUTE_NAME = "Exceptions";

        Exceptions(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        /**
         * The value of the numberOfExceptions item indicates the number of entries in the exceptionIndexTable.
         * @return number of entries in the exceptionIndexTable
         */
        [[nodiscard]]
        u2 numberOfExceptions();

        /**
         * Each value in the exceptionIndexTable array must be a valid index into the constant_pool table.
         * The constant_pool entry at that index must be a CONSTANT_Class_info structure (§4.4.1) representing a
         * class type that this method is declared to throw.
         * @return exceptionIndexTable array of exception indexes into constant pool
         */
        [[nodiscard]]
        const std::vector<u2>& exceptionIndexTable();

    private:
        std::vector<u2> m_exceptionIndexTable;
    };
}
