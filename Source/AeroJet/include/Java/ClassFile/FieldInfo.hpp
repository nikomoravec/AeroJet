/*
 * FieldInfo.hpp
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

#include "Java/ClassFile/Attributes/AttributeInfo.hpp"
#include "Types.hpp"

#include <vector>

namespace AeroJet::Java::ClassFile
{
    class FieldInfo
    {
      public:
        enum class AccessFlags : u2
        {
            ACC_PUBLIC    = 0x0001,
            ACC_PRIVATE   = 0x0002,
            ACC_PROTECTED = 0x0004,
            ACC_STATIC    = 0x0008,
            ACC_FINAL     = 0x0010,
            ACC_VOLATILE  = 0x0040,
            ACC_TRANSIENT = 0x0080,
            ACC_SYNTHETIC = 0x1000,
            ACC_ENUM      = 0x4000
        };

        FieldInfo(u2 accessFlags, u2 nameIndex, u2 descriptorIndex, const std::vector<AttributeInfo>& attributes);

        [[nodiscard]] AccessFlags accessFlags() const;

        [[nodiscard]] u2 nameIndex() const;

        [[nodiscard]] u2 descriptorIndex() const;

      protected:
        u2                         m_accessFlags;
        u2                         m_nameIndex;
        u2                         m_descriptorIndex;
        std::vector<AttributeInfo> m_attributes;
    };
} // namespace AeroJet::Java::ClassFile
