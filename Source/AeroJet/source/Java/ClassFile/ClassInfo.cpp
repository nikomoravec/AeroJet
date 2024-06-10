/*
 * ClassInfo.cpp
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

#include "Java/ClassFile/ClassInfo.hpp"

#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"
#include "Java/ClassFile/Utils/ClassInfoUtils.hpp"

namespace AeroJet::Java::ClassFile
{
    ClassInfo::ClassInfo(u2 minorVersion,
                         u2 majorVersion,
                         ConstantPool& constantPool,
                         u2 accessFlags,
                         u2 thisClass,
                         std::optional<u2> superClass,
                         const std::vector<u2>& interfaces,
                         const std::vector<FieldInfo>& fields,
                         const std::vector<MethodInfo>& methods,
                         const std::vector<AttributeInfo>& attributes) :
        m_minorVersion(minorVersion),
        m_majorVersion(majorVersion), m_constantPool(constantPool), m_accessFlags(accessFlags), m_thisClass(thisClass),
        m_superClass(superClass), m_interfaces(interfaces), m_fields(fields), m_methods(methods),
        m_attributes(attributes)
    {
    }

    u2 ClassInfo::minorVersion() const
    {
        return m_minorVersion;
    }

    u2 ClassInfo::majorVersion() const
    {
        return m_majorVersion;
    }

    const ConstantPool& ClassInfo::constantPool() const
    {
        return m_constantPool;
    }

    ClassInfo::AccessFlags ClassInfo::accessFlags() const
    {
        return static_cast<AccessFlags>(m_accessFlags);
    }

    u2 ClassInfo::thisClass() const
    {
        return m_thisClass;
    }

    bool ClassInfo::isSuperClassPresented() const
    {
        return m_superClass.has_value();
    }

    u2 ClassInfo::superClass() const
    {
        if(!isSuperClassPresented())
        {
            throw Exceptions::RuntimeException(
                fmt::format("Super Class not presented in \"{}\"", Utils::ClassInfoUtils::className(*this)));
        }

        return m_superClass.value();
    }

    const std::vector<u2>& ClassInfo::interfaces() const
    {
        return m_interfaces;
    }

    const std::vector<FieldInfo>& ClassInfo::fields() const
    {
        return m_fields;
    }

    const std::vector<MethodInfo>& ClassInfo::methods() const
    {
        return m_methods;
    }

    const std::vector<AttributeInfo>& ClassInfo::attributes() const
    {
        return m_attributes;
    }
} // namespace AeroJet::Java::ClassFile
