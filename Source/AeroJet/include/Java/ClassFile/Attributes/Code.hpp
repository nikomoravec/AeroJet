/*
 * Code.hpp
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

#include "Java/ByteCode/Instruction.hpp"
#include "Java/ClassFile/Attributes/Attribute.hpp"
#include "Java/ClassFile/Attributes/AttributeInfo.hpp"
#include "Java/ClassFile/ConstantPool.hpp"
#include "Types.hpp"
#include <vector>

namespace AeroJet::Java::ClassFile
{
    class Code : public Attribute
    {
    public:
        static constexpr auto CODE_ATTRIBUTE_NAME = "Code";

        class ExceptionTableEntry
        {
        public:
            ExceptionTableEntry(u2 startPc, u2 endPc, u2 handlePc, u2 catchType);

            [[nodiscard]]
            u2 startPc() const;

            [[nodiscard]]
            u2 endPc() const;

            [[nodiscard]]
            u2 handlerPc() const;

            [[nodiscard]]
            u2 catchType() const;

        protected:
            u2 m_startPc;
            u2 m_endPc;
            u2 m_handlerPc;
            u2 m_catchType;
        };

    public:
        Code(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        [[nodiscard]]
        u2 maxStack() const;

        [[nodiscard]]
        u2 maxLocals() const;

        [[nodiscard]]
        const std::vector<ByteCode::Instruction>& code() const;

        [[nodiscard]]
        const std::vector<Code::ExceptionTableEntry>& exceptionTable() const;

        [[nodiscard]]
        const std::vector<AttributeInfo>& attributes() const;

    protected:
        u2 m_maxStack;
        u2 m_maxLocals;
        std::vector<ByteCode::Instruction> m_code;
        std::vector<ExceptionTableEntry> m_exceptionTable;
        std::vector<AttributeInfo> m_attributes;
    };
}
