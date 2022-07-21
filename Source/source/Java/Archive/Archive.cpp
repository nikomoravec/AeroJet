#include "Java/Archive/Archive.h"
#include "Java/Archive/ClassInfo.h"
#include "Java/Archive/ConstantPoolEntry.h"
#include "Java/Archive/MethodInfo.h"
#include "Compiler/Exceptions/NotSupportedException.h"
#include "Compiler/Exceptions/UnknownConstantPoolTagException.h"
#include "fmt/format.h"

#include <bit>
#include <strstream>
#include <type_traits>

namespace
{
    template<typename T>
    T swapEndian(const T& data)
    {
        static_assert(CHAR_BIT == 8, "CHAR_BIT != 8");

        union ByteObjectRepresentation
        {
            T object;
            unsigned char bytes[sizeof(T)];
        };

        ByteObjectRepresentation source{};
        ByteObjectRepresentation dst{};

        source.object = data;

        for (size_t k = 0; k < sizeof(T); k++)
        {
            dst.bytes[k] = source.bytes[sizeof(T) - k - 1];
        }

        return dst.object;
    }
}

namespace SuperJet::Java::Archive
{
    template<>
    JVM::u2 read(std::istream& stream)
    {
        JVM::u2 read = 0;
        stream.read((char*)&read, sizeof(read));
        read = swapEndian(read);
        return read;
    }

    template<>
    JVM::u4 read(std::istream& stream)
    {
        JVM::u4 read = 0;
        stream.read((char*)&read, sizeof(read));
        read = swapEndian(read);
        return read;
    }


    template<typename T>
    T read(std::istream& stream, ConstantPoolInfoTag tag) requires std::is_base_of_v<ConstantPoolEntry, T>;

    template<>
    ConstantPoolInfoTag read(std::istream& stream)
    {
        ConstantPoolInfoTag tag;
        stream.read((char*)&tag, std::ios::cur);
        return tag;
    }

    template<>
    ConstantPoolInfoUtf8 read(std::istream& stream, ConstantPoolInfoTag tag)
    {
        const JVM::u2 readLength = read<JVM::u2>(stream);

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
    ConstantPoolInfoInteger read(std::istream& stream, ConstantPoolInfoTag tag)
    {
        const JVM::u4 readBytes = read<JVM::u4>(stream);
        return ConstantPoolInfoInteger{tag, readBytes};
    }

    template<>
    ConstantPoolInfoLong read(std::istream& stream, ConstantPoolInfoTag tag)
    {
        const JVM::u4 readHighBytes = read<JVM::u4>(stream);
        const JVM::u4 readLowBytes = read<JVM::u4>(stream);

        return ConstantPoolInfoLong{tag, readHighBytes, readLowBytes};
    }

    template<>
    ConstantPoolInfoClass read(std::istream& stream, ConstantPoolInfoTag tag)
    {
        const JVM::u2 readNameIndex  = read<JVM::u2>(stream);
        return ConstantPoolInfoClass{tag, readNameIndex};
    }

    template<>
    ConstantPoolInfoString read(std::istream& stream, ConstantPoolInfoTag tag)
    {
        const JVM::u2 readStringIndex = read<JVM::u2>(stream);
        return ConstantPoolInfoString{tag, readStringIndex};
    }

    template<>
    ConstantPoolInfoFieldRef read(std::istream& stream, ConstantPoolInfoTag tag)
    {
        const JVM::u2 readClassIndex = read<JVM::u2>(stream);
        const JVM::u2 readNameAndTypeIndex = read<JVM::u2>(stream);

        return ConstantPoolInfoFieldRef{tag, readClassIndex, readNameAndTypeIndex};
    }

    template<>
    ConstantPoolInfoNameAndType read(std::istream& stream, ConstantPoolInfoTag tag)
    {
        const JVM::u2 readNameIndex = read<JVM::u2>(stream);
        const JVM::u2 readDescriptorIndex = read<JVM::u2>(stream);

        return ConstantPoolInfoNameAndType{tag, readNameIndex, readDescriptorIndex};
    }

    template<>
    ConstantPoolInfoMethodHandle read(std::istream& stream, ConstantPoolInfoTag tag)
    {
        const JVM::u1 readReferenceKind = stream.get();
        const JVM::u2 readReferenceIndex = read<JVM::u2>(stream);

        return ConstantPoolInfoMethodHandle{tag, readReferenceKind, readReferenceIndex};
    }

    template<>
    ConstantPoolInfoMethodType read(std::istream& stream, ConstantPoolInfoTag tag)
    {
        const JVM::u2 readDescriptorIndex = read<JVM::u2>(stream);
        return ConstantPoolInfoMethodType{tag, readDescriptorIndex};
    }

    template<>
    ConstantPoolInfoInvokeDynamic read(std::istream& stream, ConstantPoolInfoTag tag)
    {
        const JVM::u2 readBootstrapMethodAttributeIndex = read<JVM::u2>(stream);
        const JVM::u2 readNameAndTypeIndex = read<JVM::u2>(stream);

        return ConstantPoolInfoInvokeDynamic{tag, readBootstrapMethodAttributeIndex, readNameAndTypeIndex};
    }

    template<>
    AttributeInfo read(std::istream& stream)
    {
        const JVM::u2 readAttributeNameIndex = read<JVM::u2>(stream);
        const JVM::u4 readAttributeInfoSize = read<JVM::u4>(stream);

        std::vector<JVM::u1> attributeInfo;
        attributeInfo.reserve(readAttributeInfoSize);
        for (int32_t attributeInfoIndex = 0; attributeInfoIndex < readAttributeInfoSize; attributeInfoIndex++)
        {
            attributeInfo.emplace_back(stream.get());
        }

        return AttributeInfo{readAttributeNameIndex, attributeInfo};
    }

    template<>
    FieldInfo read(std::istream& stream)
    {
        const JVM::u2 readAccessFlags = read<JVM::u2>(stream);
        const JVM::u2 readNameIndex = read<JVM::u2>(stream);
        const JVM::u2 readDescriptorIndex = read<JVM::u2>(stream);
        const JVM::u2 readAttributesCount = read<JVM::u2>(stream);

        std::vector<AttributeInfo> attributes;
        attributes.reserve(readAttributesCount);
        for (int32_t attributeIndex = 0; attributeIndex < readAttributesCount; attributeIndex++)
        {
            attributes.emplace_back(read<AttributeInfo>(stream));
        }

        return FieldInfo{readAccessFlags, readNameIndex, readDescriptorIndex, attributes};
    }

    template<>
    MethodInfo read(std::istream& stream)
    {
        const JVM::u2 readAccessFlags = read<JVM::u2>(stream);
        const JVM::u2 readNameIndex = read<JVM::u2>(stream);
        const JVM::u2 readDescriptorIndex = read<JVM::u2>(stream);
        const JVM::u2 readAttributesCount = read<JVM::u2>(stream);

        std::vector<AttributeInfo> readMethodAttributes;
        readMethodAttributes.reserve(readAttributesCount);
        for (int32_t attributeIndex = 0; attributeIndex < readAttributesCount; attributeIndex++)
        {
            readMethodAttributes.emplace_back(read<AttributeInfo>(stream));
        }

        return MethodInfo{readAccessFlags, readNameIndex, readDescriptorIndex, readMethodAttributes};
    }

    template<>
    ClassInfo read(std::istream& stream)
    {
        const JVM::u4 readMagic = read<JVM::u4>(stream);

        if (readMagic != JAVA_CLASS_MAGIC)
        {
            throw SuperJet::Compiler::RuntimeException("Not a Java class!");
        }

        const JVM::u2 readMinorVersion = read<JVM::u2>(stream);
        const JVM::u2 readMajorVersion = read<JVM::u2>(stream);

        if (readMajorVersion > MAX_JAVA_CLASS_MAJOR_VERSION)
        {
            throw SuperJet::Compiler::NotSupportedException(fmt::format("Unsupported java class version file {}! Maximum supported version is: {}", readMajorVersion, MAX_JAVA_CLASS_MAJOR_VERSION));
        }

        const JVM::u2 readConstantPoolSize = read<JVM::u2>(stream);
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
                    throw SuperJet::Compiler::UnknownConstantPoolTagException(tag);
            }

            constantPool.addEntry(entry);
        }

        const JVM::u2 readAccessFlags = read<JVM::u2>(stream);
        const JVM::u2 readThisClass = read<JVM::u2>(stream);
        const JVM::u2 readSuperClass = read<JVM::u2>(stream);
        const JVM::u2 readInterfacesCount = read<JVM::u2>(stream);

        std::vector<JVM::u2> interfaces;
        interfaces.reserve(readInterfacesCount);
        for (int interfaceIndex = 0; interfaceIndex < readInterfacesCount; interfaceIndex++)
        {
            interfaces.emplace_back(read<JVM::u2>(stream));
        }

        const JVM::u2 readFieldsCount = read<JVM::u2>(stream);

        std::vector<FieldInfo> fields;
        fields.reserve(readFieldsCount);
        for (int fieldIndex = 0; fieldIndex < readFieldsCount; fieldIndex++)
        {
            fields.emplace_back(read<FieldInfo>(stream));
        }

        const JVM::u2 readMethodsCount = read<JVM::u2>(stream);
        std::vector<MethodInfo> methods;
        methods.reserve(readMethodsCount);
        for (int methodIndex = 0; methodIndex < readMethodsCount; methodIndex++)
        {
            methods.emplace_back(read<MethodInfo>(stream));
        }

        const JVM::u2 readAttributesCount = read<JVM::u2>(stream);
        std::vector<AttributeInfo> attributes;
        attributes.reserve(readAttributesCount);
        for (int attributeIndex = 0; attributeIndex < readAttributesCount; attributeIndex++)
        {
            attributes.emplace_back(read<AttributeInfo>(stream));
        }

        ClassInfo classInfo {readMagic, readMinorVersion, readMajorVersion,
                         constantPool, readAccessFlags, readThisClass,
                     readSuperClass == 0 ? std::nullopt : std::optional<JVM::u2>(readSuperClass), interfaces, fields, methods,
                         attributes};

        return classInfo;
    }
}