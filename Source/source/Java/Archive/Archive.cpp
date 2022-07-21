#include "Java/Archive/Archive.h"
#include "Java/Archive/Class.h"
#include "Java/Archive/ConstantPoolEntry.h"
#include "Java/Archive/MethodInfo.h"

#include <bit>
#include <strstream>
#include <type_traits>
#include <iostream>

namespace SuperJet::Java::Archive
{
    namespace
    {
        template<typename T>
        T swapEndian(const T& u)
        {
            static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

            union ByteObjectRepresentation
            {
                T object;
                unsigned char bytes[sizeof(T)];
            };

            ByteObjectRepresentation source{};
            ByteObjectRepresentation dst{};

            source.object = u;

            for (size_t k = 0; k < sizeof(T); k++)
            {
                dst.bytes[k] = source.bytes[sizeof(T) - k - 1];
            }

            return dst.object;
        }
    }

    template<typename T>
    T read(std::basic_istream<char>& stream, ConstantPoolInfoTag tag) requires std::is_base_of_v<ConstantPoolEntry, T>;

    template<>
    ConstantPoolInfoTag read(std::basic_istream<char>& stream)
    {
        ConstantPoolInfoTag tag;
        stream.read((char*)&tag, std::ios::cur);
        return tag;
    }

    template<>
    ConstantPoolInfoUtf8 read(std::basic_istream<char>& stream, ConstantPoolInfoTag tag)
    {
        JVM::u2 readLength  = 0;
        stream.read((char*)&readLength, sizeof(readLength));
        readLength = swapEndian(readLength);

        std::vector<JVM::u1> bytes;
        bytes.reserve(readLength - 1);

        for (int32_t ByteIndex = 0; ByteIndex < readLength; ByteIndex++)
        {
            JVM::u1 byte = 0x0;
            stream.read((char*)&byte, sizeof(byte));
            bytes.emplace_back(byte);
        }

        return ConstantPoolInfoUtf8{tag, bytes};
    }

    template<>
    ConstantPoolInfoInteger read(std::basic_istream<char>& stream, ConstantPoolInfoTag tag)
    {
        JVM::u4 readBytes  = 0;
        stream.read((char*)&readBytes, sizeof(readBytes));
        readBytes = swapEndian(readBytes);

        return ConstantPoolInfoInteger{tag, readBytes};
    }

    template<>
    ConstantPoolInfoLong read(std::basic_istream<char>& stream, ConstantPoolInfoTag tag)
    {
        JVM::u4 readHighBytes  = 0;
        JVM::u4 readLowBytes  = 0;

        stream.read((char*)&readHighBytes, sizeof(readHighBytes));
        readHighBytes = swapEndian(readHighBytes);

        stream.read((char*)&readLowBytes, sizeof(readLowBytes));
        readLowBytes = swapEndian(readLowBytes);

        return ConstantPoolInfoLong{tag, readHighBytes, readLowBytes};
    }

    template<>
    ConstantPoolInfoClass read(std::basic_istream<char>& stream, ConstantPoolInfoTag tag)
    {
        JVM::u2 readNameIndex  = 0;
        stream.read((char*)&readNameIndex, sizeof(readNameIndex));
        readNameIndex = swapEndian(readNameIndex);

        return ConstantPoolInfoClass{tag, readNameIndex};
    }

    template<>
    ConstantPoolInfoString read(std::basic_istream<char>& stream, ConstantPoolInfoTag tag)
    {
        JVM::u2 readStringIndex  = 0;
        stream.read((char*)&readStringIndex, sizeof(readStringIndex));
        readStringIndex = swapEndian(readStringIndex);

        return ConstantPoolInfoString{tag, readStringIndex};
    }

    template<>
    ConstantPoolInfoFieldRef read(std::basic_istream<char>& stream, ConstantPoolInfoTag tag)
    {
        JVM::u2 readClassIndex  = 0;
        stream.read((char*)&readClassIndex, sizeof(readClassIndex));
        readClassIndex = swapEndian(readClassIndex);

        JVM::u2 readNameAndTypeIndex  = 0;
        stream.read((char*)&readNameAndTypeIndex, sizeof(readNameAndTypeIndex));
        readNameAndTypeIndex = swapEndian(readNameAndTypeIndex);

        return ConstantPoolInfoFieldRef{tag, readClassIndex, readNameAndTypeIndex};
    }

    template<>
    ConstantPoolInfoNameAndType read(std::basic_istream<char>& stream, ConstantPoolInfoTag tag)
    {
        JVM::u2 readNameIndex  = 0;
        stream.read((char*)&readNameIndex, sizeof(readNameIndex));
        readNameIndex = swapEndian(readNameIndex);

        JVM::u2 readDescriptorIndex  = 0;
        stream.read((char*)&readDescriptorIndex, sizeof(readDescriptorIndex));
        readDescriptorIndex = swapEndian(readDescriptorIndex);

        return ConstantPoolInfoNameAndType{tag, readNameIndex, readDescriptorIndex};
    }

    template<>
    ConstantPoolInfoMethodHandle read(std::basic_istream<char>& stream, ConstantPoolInfoTag tag)
    {
        JVM::u1 readReferenceKind  = 0;
        stream.read((char*)&readReferenceKind, sizeof(readReferenceKind));

        JVM::u2 readReferenceIndex  = 0;
        stream.read((char*)&readReferenceIndex, sizeof(readReferenceIndex));
        readReferenceIndex = swapEndian(readReferenceIndex);

        return ConstantPoolInfoMethodHandle{tag, readReferenceKind, readReferenceIndex};
    }

    template<>
    ConstantPoolInfoMethodType read(std::basic_istream<char>& stream, ConstantPoolInfoTag tag)
    {
        JVM::u2 readDescriptorIndex  = 0;
        stream.read((char*)&readDescriptorIndex, sizeof(readDescriptorIndex));
        readDescriptorIndex = swapEndian(readDescriptorIndex);

        return ConstantPoolInfoMethodType{tag, readDescriptorIndex};
    }

    template<>
    ConstantPoolInfoInvokeDynamic read(std::basic_istream<char>& stream, ConstantPoolInfoTag tag)
    {
        JVM::u2 readBootstrapMethodAttributeIndex  = 0;
        stream.read((char*)&readBootstrapMethodAttributeIndex, sizeof(readBootstrapMethodAttributeIndex));
        readBootstrapMethodAttributeIndex = swapEndian(readBootstrapMethodAttributeIndex);

        JVM::u2 readNameAndTypeIndex  = 0;
        stream.read((char*)&readNameAndTypeIndex, sizeof(readNameAndTypeIndex));
        readNameAndTypeIndex = swapEndian(readNameAndTypeIndex);

        return ConstantPoolInfoInvokeDynamic{tag, readBootstrapMethodAttributeIndex, readNameAndTypeIndex};
    }

    template<>
    AttributeInfo read(std::basic_istream<char>& stream)
    {
        JVM::u2 readAttributeNameIndex = 0;
        stream.read((char*)&readAttributeNameIndex, sizeof(readAttributeNameIndex));
        readAttributeNameIndex = swapEndian(readAttributeNameIndex);

        JVM::u4 readAttributeInfoSize = 0;
        stream.read((char*)&readAttributeInfoSize, sizeof(readAttributeInfoSize));
        readAttributeInfoSize = swapEndian(readAttributeInfoSize);

        std::vector<JVM::u1> attributeInfo;
        for (int32_t attributeInfoIndex = 0; attributeInfoIndex < readAttributeInfoSize; attributeInfoIndex++)
        {
            JVM::u1 readAttributeInfoByte = 0;
            stream.read((char*)&readAttributeInfoByte, sizeof(readAttributeInfoByte));

            attributeInfo.emplace_back(readAttributeInfoByte);
        }

        return AttributeInfo{readAttributeNameIndex, attributeInfo};
    }

    template<>
    FieldInfo read(std::basic_istream<char>& stream)
    {
        JVM::u2 readAccessFlags = 0;
        stream.read((char*)&readAccessFlags, sizeof(readAccessFlags));
        readAccessFlags = swapEndian(readAccessFlags);

        JVM::u2 readNameIndex = 0;
        stream.read((char*)&readNameIndex, sizeof(readNameIndex));
        readNameIndex = swapEndian(readNameIndex);

        JVM::u2 readDescriptorIndex = 0;
        stream.read((char*)&readDescriptorIndex, sizeof(readDescriptorIndex));
        readDescriptorIndex = swapEndian(readDescriptorIndex);

        JVM::u2 readAttributesCount = 0;
        stream.read((char*)&readAttributesCount, sizeof(readAttributesCount));
        readAttributesCount = swapEndian(readAttributesCount);

        std::vector<AttributeInfo> attributes;
        attributes.reserve(readAttributesCount);
        for (int32_t attributeIndex = 0; attributeIndex < readAttributesCount; attributeIndex++)
        {
            attributes.emplace_back(read<AttributeInfo>(stream));
        }

        return FieldInfo{readAccessFlags, readNameIndex, readDescriptorIndex, attributes};
    }

    template<>
    MethodInfo read(std::basic_istream<char>& stream)
    {
        JVM::u2 readAccessFlags = 0;
        stream.read((char*)&readAccessFlags, sizeof(readAccessFlags));
        readAccessFlags = swapEndian(readAccessFlags);

        JVM::u2 readNameIndex = 0;
        stream.read((char*)&readNameIndex, sizeof(readNameIndex));
        readNameIndex = swapEndian(readNameIndex);

        JVM::u2 readDescriptorIndex = 0;
        stream.read((char*)&readDescriptorIndex, sizeof(readDescriptorIndex));
        readDescriptorIndex = swapEndian(readDescriptorIndex);

        JVM::u2 readAttributesCount = 0;
        stream.read((char*)&readAttributesCount, sizeof(readAttributesCount));
        readAttributesCount = swapEndian(readAttributesCount);

        std::vector<AttributeInfo> readMethodAttributes;
        for (int32_t attributeIndex = 0; attributeIndex < readAttributesCount; attributeIndex++)
        {
            readMethodAttributes.emplace_back(read<AttributeInfo>(stream));
        }

        return MethodInfo{readAccessFlags, readNameIndex, readDescriptorIndex, readMethodAttributes};
    }

    template<>
    Class read(std::basic_istream<char>& stream)
    {
        JVM::u4 readMagic = 0;
        stream.read((char*)&readMagic, sizeof(readMagic));
        readMagic = swapEndian(readMagic);

        if (readMagic != JAVA_CLASS_MAGIC)
        {
            throw std::runtime_error("Not a Java class!");
        }

        JVM::u2 readMinorVersion = 0;
        stream.read((char*)&readMinorVersion, sizeof(readMinorVersion));
        readMinorVersion = swapEndian(readMinorVersion);

        JVM::u2 readMajorVersion = 0;
        stream.read((char*)&readMajorVersion, sizeof(readMajorVersion));
        readMajorVersion = swapEndian(readMajorVersion);

        if (readMajorVersion > MAX_JAVA_CLASS_MAJOR_VERSION)
        {
            std::strstream stringStream;
            stringStream << "Unsupported java class version file!" << " " << "Maximum supported version is: " << MAX_JAVA_CLASS_MAJOR_VERSION << " " << "but got: " << readMajorVersion;

            throw std::runtime_error(stringStream.str());
        }

        JVM::u2 readConstantPoolSize = 0;
        stream.read((char*)&readConstantPoolSize, sizeof(readConstantPoolSize));
        readConstantPoolSize = swapEndian(readConstantPoolSize);

        ConstantPool constantPool(readConstantPoolSize);

        for (int constantPoolEntryIndex = 1; constantPoolEntryIndex < readConstantPoolSize; constantPoolEntryIndex++)
        {
            ConstantPoolInfoTag tag = read<ConstantPoolInfoTag>(stream);

            std::shared_ptr<ConstantPoolEntry> entry {nullptr};

            switch (tag)
            {
                case ConstantPoolInfoTag::UTF_8:
                    entry = std::make_shared<ConstantPoolInfoUtf8>(read<ConstantPoolInfoUtf8>(stream, ConstantPoolInfoTag::UTF_8));
                    break;
                case ConstantPoolInfoTag::INTEGER:
                    entry = std::make_shared<ConstantPoolInfoInteger>(read<ConstantPoolInfoInteger>(stream, ConstantPoolInfoTag::INTEGER));
                    break;
                case ConstantPoolInfoTag::FLOAT:
                    entry = std::make_shared<ConstantPoolInfoInteger>(read<ConstantPoolInfoInteger>(stream, ConstantPoolInfoTag::FLOAT));
                    break;
                case ConstantPoolInfoTag::LONG:
                    entry = std::make_shared<ConstantPoolInfoLong>(read<ConstantPoolInfoLong>(stream, ConstantPoolInfoTag::LONG));
                    break;
                case ConstantPoolInfoTag::DOUBLE:
                    entry = std::make_shared<ConstantPoolInfoLong>(read<ConstantPoolInfoLong>(stream, ConstantPoolInfoTag::DOUBLE));
                    break;
                case ConstantPoolInfoTag::CLASS:
                    entry = std::make_shared<ConstantPoolInfoClass>(read<ConstantPoolInfoClass>(stream, ConstantPoolInfoTag::CLASS));
                    break;
                case ConstantPoolInfoTag::STRING:
                    entry = std::make_shared<ConstantPoolInfoString>(read<ConstantPoolInfoString>(stream, ConstantPoolInfoTag::STRING));
                    break;
                case ConstantPoolInfoTag::INTERFACE_METHOD_REF:
                    entry = std::make_shared<ConstantPoolInfoFieldRef>(read<ConstantPoolInfoFieldRef>(stream, ConstantPoolInfoTag::INTERFACE_METHOD_REF));
                    break;
                case ConstantPoolInfoTag::FIELD_REF:
                    entry = std::make_shared<ConstantPoolInfoFieldRef>(read<ConstantPoolInfoFieldRef>(stream, ConstantPoolInfoTag::FIELD_REF));
                    break;
                case ConstantPoolInfoTag::METHOD_REF:
                    entry = std::make_shared<ConstantPoolInfoFieldRef>(read<ConstantPoolInfoFieldRef>(stream, ConstantPoolInfoTag::METHOD_REF));
                    break;
                case ConstantPoolInfoTag::NAME_AND_TYPE:
                    entry = std::make_shared<ConstantPoolInfoNameAndType>(read<ConstantPoolInfoNameAndType>(stream, ConstantPoolInfoTag::NAME_AND_TYPE));
                    break;
                case ConstantPoolInfoTag::METHOD_HANDLE:
                    entry = std::make_shared<ConstantPoolInfoMethodHandle>(read<ConstantPoolInfoMethodHandle>(stream, ConstantPoolInfoTag::METHOD_HANDLE));
                    break;
                case ConstantPoolInfoTag::METHOD_TYPE:
                    entry = std::make_shared<ConstantPoolInfoMethodType>(read<ConstantPoolInfoMethodType>(stream, ConstantPoolInfoTag::METHOD_TYPE));
                    break;
                case ConstantPoolInfoTag::INVOKE_DYNAMIC:
                    entry = std::make_shared<ConstantPoolInfoInvokeDynamic>(read<ConstantPoolInfoInvokeDynamic>(stream, ConstantPoolInfoTag::INVOKE_DYNAMIC));
                    break;
                default:
                    throw std::runtime_error("Unknown type");
            }

            constantPool.addEntry(entry);
        }

        JVM::u2 readAccessFlags = 0;
        stream.read((char*)&readAccessFlags, sizeof(readAccessFlags));
        readAccessFlags = swapEndian(readAccessFlags);

        JVM::u2 readThisClass = 0;
        stream.read((char*)&readThisClass, sizeof(readThisClass));
        readThisClass = swapEndian(readThisClass);

        JVM::u2 readSuperClass = 0;
        stream.read((char*)&readSuperClass, sizeof(readSuperClass));
        readSuperClass = swapEndian(readSuperClass);

        JVM::u2 readInterfacesCount = 0;
        stream.read((char*)&readInterfacesCount, sizeof(readInterfacesCount));
        readInterfacesCount = swapEndian(readInterfacesCount);

        std::vector<JVM::u2> interfaces;
        for (int interfaceIndex = 0; interfaceIndex < readInterfacesCount; interfaceIndex++)
        {
            JVM::u2 interface = 0;
            stream.read((char*)&interface, sizeof(interface));
            interface = swapEndian(interface);

            interfaces.emplace_back(interface);
        }

        JVM::u2 readFieldsCount = 0;
        stream.read((char*)&readFieldsCount, sizeof(readFieldsCount));
        readFieldsCount = swapEndian(readFieldsCount);

        std::vector<FieldInfo> fields;
        fields.reserve(readFieldsCount);
        for (int fieldIndex = 0; fieldIndex < readFieldsCount; fieldIndex++)
        {
            fields.emplace_back(read<FieldInfo>(stream));
        }

        JVM::u2 readMethodsCount = 0;
        stream.read((char*)&readMethodsCount, sizeof(readMethodsCount));
        readMethodsCount = swapEndian(readMethodsCount);

        std::vector<MethodInfo> methods;
        methods.reserve(readMethodsCount);
        for (int methodIndex = 0; methodIndex < readMethodsCount; methodIndex++)
        {
            methods.emplace_back(read<MethodInfo>(stream));
        }

        JVM::u2 readAttributesCount = 0;
        stream.read((char*)&readAttributesCount, sizeof(readAttributesCount));
        readAttributesCount = swapEndian(readAttributesCount);

        std::vector<AttributeInfo> attributes;
        attributes.reserve(readAttributesCount);
        for (int attributeIndex = 0; attributeIndex < readAttributesCount; attributeIndex++)
        {
            attributes.emplace_back(read<AttributeInfo>(stream));
        }

        Class clazz {readMagic, readMinorVersion, readMajorVersion,
                     constantPool, readAccessFlags, readThisClass,
                     readSuperClass, interfaces, fields, methods,
                     attributes};

        return clazz;
    }
}