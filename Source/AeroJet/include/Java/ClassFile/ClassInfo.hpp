/*
 * ClassInfo.hpp
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
#include "Java/ClassFile/ConstantPool.hpp"
#include "Java/ClassFile/FieldInfo.hpp"
#include "Java/ClassFile/MethodInfo.hpp"
#include "Types.hpp"
#include <optional>
#include <vector>

namespace AeroJet::Java::ClassFile
{
    class ClassInfo
    {
    public:
        static constexpr u4 JAVA_CLASS_MAGIC  = 0xCAFEBABE;

        enum class AccessFlags : u2
        {
            ACC_PUBLIC     = 0x0001,
            ACC_FINAL      = 0x0010,
            ACC_SUPER      = 0x0020,
            ACC_INTERFACE  = 0x0200,
            ACC_ABSTRACT   = 0x0400,
            ACC_SYNTHETIC  = 0x1000,
            ACC_ANNOTATION = 0x2000,
            ACC_ENUM       = 0x4000
        };

        ClassInfo(u2 minorVersion,
                  u2 majorVersion,
                  ConstantPool& constantPool,
                  u2 accessFlags, u2 thisClass,
                  std::optional<u2> superClass,
                  const std::vector<u2>& interfaces,
                  const std::vector<FieldInfo>& fields,
                  const std::vector<MethodInfo>& methods,
                  const std::vector<AttributeInfo>& attributes);

        [[nodiscard]]
        u2 minorVersion() const;

        [[nodiscard]]
        u2 majorVersion() const;

        [[nodiscard]]
        const ConstantPool& constantPool() const;

        [[nodiscard]]
        AccessFlags accessFlags() const;

        [[nodiscard]]
        u2 thisClass() const;

        [[nodiscard]]
        bool isSuperClassPresented() const;

        [[nodiscard]]
        u2 superClass() const;

        [[nodiscard]]
        const std::vector<u2>& interfaces() const;

        [[nodiscard]]
        const std::vector<FieldInfo>& fields() const;

        [[nodiscard]]
        const std::vector<MethodInfo>& methods() const;

        [[nodiscard]]
        const std::vector<AttributeInfo>& attributes() const;

    protected:
        u2 m_minorVersion;
        u2 m_majorVersion;
        ConstantPool m_constantPool;
        u2 m_accessFlags;
        u2 m_thisClass;
        std::optional<u2> m_superClass;
        std::vector<u2> m_interfaces;
        std::vector<FieldInfo> m_fields;
        std::vector<MethodInfo> m_methods;
        std::vector<AttributeInfo> m_attributes;
    };
}
