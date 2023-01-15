/*
 * ClassInfo.cpp
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

#include "Java/ClassFile/ClassInfo.hpp"
#include "Java/ClassFile/Utils/ClassInfoUtils.hpp"
#include "Exceptions/RuntimeException.hpp"
#include "fmt/format.h"
#include "Stream/Reader.hpp"

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
                         const std::vector<AttributeInfo> &attributes)  :
            m_minorVersion(minorVersion),
            m_majorVersion(majorVersion),
            m_constantPool(constantPool),
            m_accessFlags(accessFlags),
            m_thisClass(thisClass),
            m_superClass(superClass),
            m_interfaces(interfaces),
            m_fields(fields),
            m_methods(methods),
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
        if (!isSuperClassPresented())
        {
            throw Exceptions::RuntimeException(fmt::format("Super Class not presented in \"{}\"",
                                                           Utils::ClassInfoUtils::className(*this)));
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
}

template<>
AeroJet::Java::ClassFile::ClassInfo AeroJet::Stream::Reader::read(std::istream& stream, ByteOrder byteOrder)
{
    static constexpr AeroJet::u2 MAX_JAVA_CLASS_MAJOR_VERSION = 52;

    const AeroJet::u4 magic = AeroJet::Stream::Reader::read<AeroJet::u4>(stream, byteOrder);
    if (magic != AeroJet::Java::ClassFile::ClassInfo::JAVA_CLASS_MAGIC)
    {
        throw AeroJet::Exceptions::RuntimeException("Not a Java class!");
    }

    const AeroJet::u2 minorVersion = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);
    const AeroJet::u2 majorVersion = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);

    if (majorVersion > MAX_JAVA_CLASS_MAJOR_VERSION)
    {
        throw Exceptions::RuntimeException(
                fmt::format("Unsupported java class version file {}! Maximum supported version is: {}", majorVersion, MAX_JAVA_CLASS_MAJOR_VERSION)
        );
    }

    const AeroJet::u2 constantPoolSize = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);
    AeroJet::Java::ClassFile::ConstantPool constantPool;
    for (int constantPoolEntryIndex = 1; constantPoolEntryIndex < constantPoolSize; constantPoolEntryIndex++)
    {
        AeroJet::Java::ClassFile::ConstantPoolEntry entry = AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::ConstantPoolEntry>(stream, byteOrder);
        const AeroJet::Java::ClassFile::ConstantPoolInfoTag tag = entry.tag();

        constantPool.addEntry(constantPoolEntryIndex, std::move(entry));

        if (tag == AeroJet::Java::ClassFile::ConstantPoolInfoTag::LONG || tag == AeroJet::Java::ClassFile::ConstantPoolInfoTag::DOUBLE)
        {
            constantPoolEntryIndex++;
        }
    }

    const AeroJet::u2 accessFlags = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);
    const AeroJet::u2 thisClass = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);
    const AeroJet::u2 superClass = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);
    const AeroJet::u2 interfacesCount = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);

    std::vector<AeroJet::u2> interfaces;
    interfaces.reserve(interfacesCount);
    for (int interfaceIndex = 0; interfaceIndex < interfacesCount; interfaceIndex++)
    {
        interfaces.emplace_back(AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder));
    }

    const AeroJet::u2 fieldsCount = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);
    std::vector<AeroJet::Java::ClassFile::FieldInfo> fields;
    fields.reserve(fieldsCount);
    for (int fieldIndex = 0; fieldIndex < fieldsCount; fieldIndex++)
    {
        fields.emplace_back(AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::FieldInfo>(stream, byteOrder));
    }

    const AeroJet::u2 readMethodsCount = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);
    std::vector<AeroJet::Java::ClassFile::MethodInfo> methods;
    methods.reserve(readMethodsCount);
    for (int methodIndex = 0; methodIndex < readMethodsCount; methodIndex++)
    {
        methods.emplace_back(AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::MethodInfo>(stream, byteOrder));
    }

    const AeroJet::u2 readAttributesCount = AeroJet::Stream::Reader::read<AeroJet::u2>(stream, byteOrder);
    std::vector<AeroJet::Java::ClassFile::AttributeInfo> attributes;
    attributes.reserve(readAttributesCount);
    for (int attributeIndex = 0; attributeIndex < readAttributesCount; attributeIndex++)
    {
        attributes.emplace_back(AeroJet::Stream::Reader::read<AeroJet::Java::ClassFile::AttributeInfo>(stream, byteOrder));
    }

    return {minorVersion,
            majorVersion,
            constantPool,
            accessFlags,
            thisClass,
            superClass == 0 ? std::nullopt : std::optional<AeroJet::u2>(superClass),
            interfaces,
            fields,
            methods,
            attributes};
}
