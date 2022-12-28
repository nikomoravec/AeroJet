/*
 * ConstantPoolEntry.hpp
 *
 * Copyright © 2022 AeroJet Developers. All Rights Reserved.
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
#include <string>
#include <utility>
#include <vector>

namespace AeroJet::Java::ClassFile
{
    enum class ConstantPoolInfoTag : u1
    {
        UTF_8 = 1,
        INTEGER = 3,
        FLOAT = 4,
        LONG = 5,
        DOUBLE = 6,
        CLASS = 7,
        STRING = 8,
        FIELD_REF = 9,
        METHOD_REF = 10,
        INTERFACE_METHOD_REF = 11,
        NAME_AND_TYPE = 12,
        METHOD_HANDLE = 15,
        METHOD_TYPE = 16,
        INVOKE_DYNAMIC = 18
    };

    class ConstantPoolEntry
    {
    public:
        ConstantPoolEntry(ConstantPoolInfoTag tag, const std::vector<u1>& data);

        [[nodiscard]]
        ConstantPoolInfoTag tag() const;

        [[nodiscard]]
        const std::vector<u1>& data() const;

    protected:
        ConstantPoolInfoTag m_tag;
        std::vector<u1> m_data;
    };

    namespace ConstantPoolInfoUtf8
    {
        [[nodiscard]]
        u2 length(const ConstantPoolEntry& entry);
    }

    namespace ConstantPoolInfoClass
    {
        [[nodiscard]]
        u2 nameIndex(const ConstantPoolEntry& entry);
    }

    namespace ConstantPoolInfoFieldRef
    {
        [[nodiscard]]
        u2 classIndex(const ConstantPoolEntry& entry);

        [[nodiscard]]
        u2 nameAndTypeIndex(const ConstantPoolEntry& entry);
    }

    namespace ConstantPoolInfoMethodRef = ConstantPoolInfoFieldRef;
    namespace ConstantPoolInfoInterfaceMethodRef = ConstantPoolInfoFieldRef;

    namespace ConstantPoolInfoString
    {
        [[nodiscard]]
        u2 stringIndex(const ConstantPoolEntry& entry);
    }

    namespace ConstantPoolInfoInteger
    {
        [[nodiscard]]
        u4 bytes(const ConstantPoolEntry& entry);
    }

    namespace ConstantPoolFloat = ConstantPoolInfoInteger;

    namespace ConstantPoolInfoLong
    {
        [[nodiscard]]
        u4 highBytes(const ConstantPoolEntry& entry);

        [[nodiscard]]
        u4 lowBytes(const ConstantPoolEntry& entry);
    }

    namespace ConstantPoolInfoDouble = ConstantPoolInfoLong;

    namespace ConstantPoolInfoNameAndType
    {
        [[nodiscard]]
        u2 nameIndex(const ConstantPoolEntry& entry);

        [[nodiscard]]
        u2 descriptorIndex(const ConstantPoolEntry& entry);
    }

    namespace ConstantPoolInfoMethodHandle
    {
        enum class ReferenceKind : u1
        {
            REF_getField = 1,
            REF_getStatic = 2,
            REF_putField = 3,
            REF_putStatic = 4,
            REF_invokeVirtual = 5,
            REF_invokeStatic = 6,
            REF_invokeSpecial = 7,
            REF_newInvokeSpecial = 8,
            REF_invokeInterface = 9
        };

        [[nodiscard]]
        ReferenceKind referenceKind(const ConstantPoolEntry& entry);

        [[nodiscard]]
        u2 referenceIndex(const ConstantPoolEntry& entry);
    }

    namespace ConstantPoolInfoMethodType
    {
        [[nodiscard]]
        u2 descriptorIndex(const ConstantPoolEntry& entry);
    }

    namespace ConstantPoolInfoInvokeDynamic
    {
        [[nodiscard]]
        u2 bootstrapMethodAttributeIndex(const ConstantPoolEntry& entry);

        [[nodiscard]]
        u2 nameAndTypeIndex(const ConstantPoolEntry& entry);
    };
}
