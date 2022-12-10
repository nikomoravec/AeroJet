#include "Java/Archive/Archive.h"
#include "Java/Archive/ClassInfo.h"
#include "Java/Archive/ConstantPoolEntry.h"
#include "Java/Archive/MethodInfo.h"
#include "Java/JVM/Runtime/Operations.h"
#include "Compiler/Exceptions/NotSupportedException.h"
#include "Compiler/Exceptions/OperationNotSupportedException.h"
#include "Compiler/Exceptions/UnknownConstantPoolTagException.h"
#include "fmt/format.h"

#include <bit>
#include <strstream>
#include <type_traits>
#include "spdlog/spdlog.h"

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
    template<typename T>
    static inline T readInternal(std::istream& stream)
    {
        if (stream.eof())
        {
            throw Compiler::RuntimeException(fmt::format("stream EOF at {:#08x}! Read size was {}", stream.tellg(), sizeof(T)));
        }
        
        T read {};
        stream.read((char*)&read, sizeof(read));

        if (sizeof(T) > 1)
        {
            read = swapEndian(read);
        }

        return read;
    }

    template<>
    JVM::u1 read(std::istream& stream)
    {
        return readInternal<JVM::u1>(stream);
    }

    template<>
    JVM::u2 read(std::istream& stream)
    {
        return readInternal<JVM::u2>(stream);
    }

    template<>
    JVM::u4 read(std::istream& stream)
    {
        return readInternal<JVM::u4>(stream);
    }

    template<>
    JVM::i4 read(std::istream& stream)
    {
        return readInternal<JVM::i4>(stream);
    }


    template<typename T>
    T read(std::istream& stream, ConstantPoolInfoTag tag) requires std::is_base_of_v<ConstantPoolEntry, T>;

    template<>
    ConstantPoolInfoTag read(std::istream& stream)
    {
        return readInternal<ConstantPoolInfoTag>(stream);
    }

    template<>
    ConstantPoolInfoUtf8 read(std::istream& stream, ConstantPoolInfoTag tag)
    {
        const JVM::u2 readLength = read<JVM::u2>(stream);

        std::vector<JVM::u1> bytes;
        bytes.reserve(readLength);

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
        const JVM::u1 readReferenceKind = read<JVM::u1>(stream);
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
            attributeInfo.emplace_back(read<JVM::u1>(stream));
        }

        return AttributeInfo{readAttributeNameIndex, attributeInfo, static_cast<JVM::u4>(stream.tellg())};
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
            switch (tag)
            {
                case ConstantPoolInfoTag::UTF_8:
                    constantPool.addEntry(constantPoolEntryIndex, std::make_shared<ConstantPoolInfoUtf8>(read<ConstantPoolInfoUtf8>(stream, ConstantPoolInfoTag::UTF_8)));
                    break;
                case ConstantPoolInfoTag::INTEGER:
                    constantPool.addEntry(constantPoolEntryIndex, std::make_shared<ConstantPoolInfoInteger>(read<ConstantPoolInfoInteger>(stream, ConstantPoolInfoTag::INTEGER)));
                    break;
                case ConstantPoolInfoTag::FLOAT:
                    constantPool.addEntry(constantPoolEntryIndex, std::make_shared<ConstantPoolInfoInteger>(read<ConstantPoolInfoInteger>(stream, ConstantPoolInfoTag::FLOAT)));
                    break;
                case ConstantPoolInfoTag::LONG:
                {
                    constantPool.addEntry(constantPoolEntryIndex, std::make_shared<ConstantPoolInfoLong>(read<ConstantPoolInfoLong>(stream, ConstantPoolInfoTag::LONG)));
                    constantPoolEntryIndex++;
                    break;
                }
                case ConstantPoolInfoTag::DOUBLE:
                {
                    constantPool.addEntry(constantPoolEntryIndex, std::make_shared<ConstantPoolInfoLong>(read<ConstantPoolInfoLong>(stream, ConstantPoolInfoTag::DOUBLE)));
                    constantPoolEntryIndex++;
                    break;
                }
                case ConstantPoolInfoTag::CLASS:
                    constantPool.addEntry(constantPoolEntryIndex, std::make_shared<ConstantPoolInfoClass>(read<ConstantPoolInfoClass>(stream, ConstantPoolInfoTag::CLASS)));
                    break;
                case ConstantPoolInfoTag::STRING:
                    constantPool.addEntry(constantPoolEntryIndex, std::make_shared<ConstantPoolInfoString>(read<ConstantPoolInfoString>(stream, ConstantPoolInfoTag::STRING)));
                    break;
                case ConstantPoolInfoTag::INTERFACE_METHOD_REF:
                    constantPool.addEntry(constantPoolEntryIndex, std::make_shared<ConstantPoolInfoFieldRef>(read<ConstantPoolInfoFieldRef>(stream, ConstantPoolInfoTag::INTERFACE_METHOD_REF)));
                    break;
                case ConstantPoolInfoTag::FIELD_REF:
                    constantPool.addEntry(constantPoolEntryIndex, std::make_shared<ConstantPoolInfoFieldRef>(read<ConstantPoolInfoFieldRef>(stream, ConstantPoolInfoTag::FIELD_REF)));
                    break;
                case ConstantPoolInfoTag::METHOD_REF:
                    constantPool.addEntry(constantPoolEntryIndex, std::make_shared<ConstantPoolInfoFieldRef>(read<ConstantPoolInfoFieldRef>(stream, ConstantPoolInfoTag::METHOD_REF)));
                    break;
                case ConstantPoolInfoTag::NAME_AND_TYPE:
                    constantPool.addEntry(constantPoolEntryIndex, std::make_shared<ConstantPoolInfoNameAndType>(read<ConstantPoolInfoNameAndType>(stream, ConstantPoolInfoTag::NAME_AND_TYPE)));
                    break;
                case ConstantPoolInfoTag::METHOD_HANDLE:
                    constantPool.addEntry(constantPoolEntryIndex, std::make_shared<ConstantPoolInfoMethodHandle>(read<ConstantPoolInfoMethodHandle>(stream, ConstantPoolInfoTag::METHOD_HANDLE)));
                    break;
                case ConstantPoolInfoTag::METHOD_TYPE:
                    constantPool.addEntry(constantPoolEntryIndex, std::make_shared<ConstantPoolInfoMethodType>(read<ConstantPoolInfoMethodType>(stream, ConstantPoolInfoTag::METHOD_TYPE)));
                    break;
                case ConstantPoolInfoTag::INVOKE_DYNAMIC:
                    constantPool.addEntry(constantPoolEntryIndex, std::make_shared<ConstantPoolInfoInvokeDynamic>(read<ConstantPoolInfoInvokeDynamic>(stream, ConstantPoolInfoTag::INVOKE_DYNAMIC)));
                    break;
                default:
                    throw SuperJet::Compiler::UnknownConstantPoolTagException(tag);
            }
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

    template<>
    std::shared_ptr<SuperJet::Java::JVM::Runtime::Operation> read(std::istream& stream, JVM::u4 firstInstructionOffset, JVM::u4 firstOpCodeLocalOffset)
    {
        std::vector<JVM::u1> data;
        SuperJet::Java::JVM::Runtime::OperationCode opCode = static_cast<SuperJet::Java::JVM::Runtime::OperationCode>(read<JVM::u1>(stream));
        spdlog::info(fmt::format("opcode {:#04x} pos {:#04x} offset {:#08x}", static_cast<JVM::u1>(opCode), stream.tellg(), firstInstructionOffset + static_cast<JVM::u4>(stream.tellg())));
        switch (opCode)
        {
            case JVM::Runtime::OperationCode::aaload:
                return std::make_shared<JVM::Runtime::aaload>();
            case JVM::Runtime::OperationCode::aastore:
                return std::make_shared<JVM::Runtime::aastore>();
            case JVM::Runtime::OperationCode::aconst_null:
                return std::make_shared<JVM::Runtime::aconst_null>();
            case JVM::Runtime::OperationCode::aload:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::aload>(data);
            case JVM::Runtime::OperationCode::aload_0:
                return std::make_shared<JVM::Runtime::aload_0>();
            case JVM::Runtime::OperationCode::aload_1:
                return std::make_shared<JVM::Runtime::aload_1>();
            case JVM::Runtime::OperationCode::aload_2:
                return std::make_shared<JVM::Runtime::aload_2>();
            case JVM::Runtime::OperationCode::aload_3:
                return std::make_shared<JVM::Runtime::aload_3>();
            case JVM::Runtime::OperationCode::anewarray:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));

                return std::make_shared<JVM::Runtime::anewarray>(data);
            }
            case JVM::Runtime::OperationCode::areturn:
                return std::make_shared<JVM::Runtime::areturn>();
            case JVM::Runtime::OperationCode::arraylength:
                return std::make_shared<JVM::Runtime::arraylength>();
            case JVM::Runtime::OperationCode::astore:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::astore>(data);
            case JVM::Runtime::OperationCode::astore_0:
                return std::make_shared<JVM::Runtime::astore_0>();
            case JVM::Runtime::OperationCode::astore_1:
                return std::make_shared<JVM::Runtime::astore_1>();
            case JVM::Runtime::OperationCode::astore_2:
                return std::make_shared<JVM::Runtime::astore_2>();
            case JVM::Runtime::OperationCode::astore_3:
                return std::make_shared<JVM::Runtime::astore_3>();
            case JVM::Runtime::OperationCode::athrow:
                return std::make_shared<JVM::Runtime::athrow>();
            case JVM::Runtime::OperationCode::baload:
                return std::make_shared<JVM::Runtime::baload>();
            case JVM::Runtime::OperationCode::bastore:
                return std::make_shared<JVM::Runtime::bastore>();
            case JVM::Runtime::OperationCode::bipush:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::bipush>(data);
            case JVM::Runtime::OperationCode::caload:
                return std::make_shared<JVM::Runtime::caload>();
            case JVM::Runtime::OperationCode::castore:
                return std::make_shared<JVM::Runtime::castore>();
            case JVM::Runtime::OperationCode::checkcast:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));

                return std::make_shared<JVM::Runtime::checkcast>(data);
            }
            case JVM::Runtime::OperationCode::d2f:
                return std::make_shared<JVM::Runtime::d2f>();
            case JVM::Runtime::OperationCode::d2i:
                return std::make_shared<JVM::Runtime::d2i>();
            case JVM::Runtime::OperationCode::d2l:
                return std::make_shared<JVM::Runtime::d2l>();
            case JVM::Runtime::OperationCode::dadd:
                return std::make_shared<JVM::Runtime::dadd>();
            case JVM::Runtime::OperationCode::daload:
                return std::make_shared<JVM::Runtime::daload>();
            case JVM::Runtime::OperationCode::dastore:
                return std::make_shared<JVM::Runtime::dastore>();
            case JVM::Runtime::OperationCode::dcmpg:
                return std::make_shared<JVM::Runtime::dcmpg>();
            case JVM::Runtime::OperationCode::dcmpl:
                return std::make_shared<JVM::Runtime::dcmpl>();
            case JVM::Runtime::OperationCode::dconst_0:
                return std::make_shared<JVM::Runtime::dconst_0>();
            case JVM::Runtime::OperationCode::dconst_1:
                return std::make_shared<JVM::Runtime::dconst_1>();
            case JVM::Runtime::OperationCode::ddiv:
                return std::make_shared<JVM::Runtime::ddiv>();
            case JVM::Runtime::OperationCode::dload:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::dload>(data);
            case JVM::Runtime::OperationCode::dload_0:
                return std::make_shared<JVM::Runtime::dload_0>();
            case JVM::Runtime::OperationCode::dload_1:
                return std::make_shared<JVM::Runtime::dload_1>();
            case JVM::Runtime::OperationCode::dload_2:
                return std::make_shared<JVM::Runtime::dload_2>();
            case JVM::Runtime::OperationCode::dload_3:
                return std::make_shared<JVM::Runtime::dload_3>();
            case JVM::Runtime::OperationCode::dmul:
                return std::make_shared<JVM::Runtime::dmul>();
            case JVM::Runtime::OperationCode::dneg:
                return std::make_shared<JVM::Runtime::dneg>();
            case JVM::Runtime::OperationCode::drem:
                return std::make_shared<JVM::Runtime::drem>();
            case JVM::Runtime::OperationCode::dreturn:
                return std::make_shared<JVM::Runtime::dreturn>();
            case JVM::Runtime::OperationCode::dstore:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::dstore>(data);
            case JVM::Runtime::OperationCode::dstore_0:
                return std::make_shared<JVM::Runtime::dstore_0>();
            case JVM::Runtime::OperationCode::dstore_1:
                return std::make_shared<JVM::Runtime::dstore_1>();
            case JVM::Runtime::OperationCode::dstore_2:
                return std::make_shared<JVM::Runtime::dstore_2>();
            case JVM::Runtime::OperationCode::dstore_3:
                return std::make_shared<JVM::Runtime::dstore_3>();
            case JVM::Runtime::OperationCode::dsub:
                return std::make_shared<JVM::Runtime::dsub>();
            case JVM::Runtime::OperationCode::dup:
                return std::make_shared<JVM::Runtime::dup>();
            case JVM::Runtime::OperationCode::dup_x1:
                return std::make_shared<JVM::Runtime::dup_x1>();
            case JVM::Runtime::OperationCode::dup_x2:
                return std::make_shared<JVM::Runtime::dup_x2>();
            case JVM::Runtime::OperationCode::dup2:
                return std::make_shared<JVM::Runtime::dup2>();
            case JVM::Runtime::OperationCode::dup2_x1:
                return std::make_shared<JVM::Runtime::dup2_x1>();
            case JVM::Runtime::OperationCode::dup2_x2:
                return std::make_shared<JVM::Runtime::dup2_x2>();
            case JVM::Runtime::OperationCode::f2i:
                return std::make_shared<JVM::Runtime::f2i>();
            case JVM::Runtime::OperationCode::f2d:
                return std::make_shared<JVM::Runtime::f2d>();
            case JVM::Runtime::OperationCode::f2l:
                return std::make_shared<JVM::Runtime::f2l>();
            case JVM::Runtime::OperationCode::fadd:
                return std::make_shared<JVM::Runtime::fadd>();
            case JVM::Runtime::OperationCode::faload:
                return std::make_shared<JVM::Runtime::faload>();
            case JVM::Runtime::OperationCode::fastore:
                return std::make_shared<JVM::Runtime::fastore>();
            case JVM::Runtime::OperationCode::fcmpg:
                return std::make_shared<JVM::Runtime::fcmpg>();
            case JVM::Runtime::OperationCode::fcmpl:
                return std::make_shared<JVM::Runtime::fcmpl>();
            case JVM::Runtime::OperationCode::fconst_0:
                return std::make_shared<JVM::Runtime::fconst_0>();
            case JVM::Runtime::OperationCode::fconst_1:
                return std::make_shared<JVM::Runtime::fconst_1>();
            case JVM::Runtime::OperationCode::fconst_2:
                return std::make_shared<JVM::Runtime::fconst_2>();
            case JVM::Runtime::OperationCode::fdiv:
                return std::make_shared<JVM::Runtime::fdiv>();
            case JVM::Runtime::OperationCode::fload:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::fload>(data);
            case JVM::Runtime::OperationCode::fload_0:
                return std::make_shared<JVM::Runtime::fload_0>();
            case JVM::Runtime::OperationCode::fload_1:
                return std::make_shared<JVM::Runtime::fload_1>();
            case JVM::Runtime::OperationCode::fload_2:
                return std::make_shared<JVM::Runtime::fload_2>();
            case JVM::Runtime::OperationCode::fload_3:
                return std::make_shared<JVM::Runtime::fload_3>();
            case JVM::Runtime::OperationCode::fmul:
                return std::make_shared<JVM::Runtime::fmul>();
            case JVM::Runtime::OperationCode::fneg:
                return std::make_shared<JVM::Runtime::fneg>();
            case JVM::Runtime::OperationCode::frem:
                return std::make_shared<JVM::Runtime::frem>();
            case JVM::Runtime::OperationCode::freturn:
                return std::make_shared<JVM::Runtime::freturn>();
            case JVM::Runtime::OperationCode::fstore:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::fstore>(data);
            case JVM::Runtime::OperationCode::fstore_0:
                return std::make_shared<JVM::Runtime::fstore_0>();
            case JVM::Runtime::OperationCode::fstore_1:
                return std::make_shared<JVM::Runtime::fstore_1>();
            case JVM::Runtime::OperationCode::fstore_2:
                return std::make_shared<JVM::Runtime::fstore_2>();
            case JVM::Runtime::OperationCode::fstore_3:
                return std::make_shared<JVM::Runtime::fstore_3>();
            case JVM::Runtime::OperationCode::fsub:
                return std::make_shared<JVM::Runtime::fsub>();
            case JVM::Runtime::OperationCode::getfield:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::getfield>(data);
            }
            case JVM::Runtime::OperationCode::getstatic:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::getstatic>(data);
            }
            case JVM::Runtime::OperationCode::GOTO:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::GOTO>(data);
            }
            case JVM::Runtime::OperationCode::goto_w:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::goto_w>(data);
            }
            case JVM::Runtime::OperationCode::i2b:
                return std::make_shared<JVM::Runtime::i2b>();
            case JVM::Runtime::OperationCode::i2c:
                return std::make_shared<JVM::Runtime::i2c>();
            case JVM::Runtime::OperationCode::i2d:
                return std::make_shared<JVM::Runtime::i2d>();
            case JVM::Runtime::OperationCode::i2f:
                return std::make_shared<JVM::Runtime::i2f>();
            case JVM::Runtime::OperationCode::i2l:
                return std::make_shared<JVM::Runtime::i2l>();
            case JVM::Runtime::OperationCode::i2s:
                return std::make_shared<JVM::Runtime::i2s>();
            case JVM::Runtime::OperationCode::iadd:
                return std::make_shared<JVM::Runtime::iadd>();
            case JVM::Runtime::OperationCode::iaload:
                return std::make_shared<JVM::Runtime::iaload>();
            case JVM::Runtime::OperationCode::iand:
                return std::make_shared<JVM::Runtime::iand>();
            case JVM::Runtime::OperationCode::iastore:
                return std::make_shared<JVM::Runtime::iastore>();
            case JVM::Runtime::OperationCode::iconst_m1:
                return std::make_shared<JVM::Runtime::iconst_m1>();
            case JVM::Runtime::OperationCode::iconst_0:
                return std::make_shared<JVM::Runtime::iconst_0>();
            case JVM::Runtime::OperationCode::iconst_1:
                return std::make_shared<JVM::Runtime::iconst_1>();
            case JVM::Runtime::OperationCode::iconst_2:
                return std::make_shared<JVM::Runtime::iconst_2>();
            case JVM::Runtime::OperationCode::iconst_3:
                return std::make_shared<JVM::Runtime::iconst_3>();
            case JVM::Runtime::OperationCode::iconst_4:
                return std::make_shared<JVM::Runtime::iconst_4>();
            case JVM::Runtime::OperationCode::iconst_5:
                return std::make_shared<JVM::Runtime::iconst_5>();
            case JVM::Runtime::OperationCode::idiv:
                return std::make_shared<JVM::Runtime::idiv>();
            case JVM::Runtime::OperationCode::if_acmpeq:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::if_acmpeq>(data);
            }
            case JVM::Runtime::OperationCode::if_acmpne:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::if_acmpne>(data);
            }
            case JVM::Runtime::OperationCode::if_icmpeq:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::if_icmpeq>(data);
            }
            case JVM::Runtime::OperationCode::if_icmpne:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::if_icmpne>(data);
            }
            case JVM::Runtime::OperationCode::if_icmplt:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::if_icmplt>(data);
            }
            case JVM::Runtime::OperationCode::if_icmpge:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::if_icmpge>(data);
            }
            case JVM::Runtime::OperationCode::if_icmpgt:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::if_icmpgt>(data);
            }
            case JVM::Runtime::OperationCode::if_icmple:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::if_icmple>(data);
            }
            case JVM::Runtime::OperationCode::ifeq:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::ifeq>(data);
            }
            case JVM::Runtime::OperationCode::ifne:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::ifne>(data);
            }
            case JVM::Runtime::OperationCode::iflt:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::iflt>(data);
            }
            case JVM::Runtime::OperationCode::ifge:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::ifge>(data);
            }
            case JVM::Runtime::OperationCode::ifgt:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::ifgt>(data);
            }
            case JVM::Runtime::OperationCode::ifle:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::ifle>(data);
            }
            case JVM::Runtime::OperationCode::ifnonnull:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::ifnonnull>(data);
            }
            case JVM::Runtime::OperationCode::ifnull:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::ifnull>(data);
            }
            case JVM::Runtime::OperationCode::iinc:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::iinc>(data);
            }
            case JVM::Runtime::OperationCode::iload:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::iload>(data);
            case JVM::Runtime::OperationCode::iload_0:
                return std::make_shared<JVM::Runtime::iload_0>();
            case JVM::Runtime::OperationCode::iload_1:
                return std::make_shared<JVM::Runtime::iload_1>();
            case JVM::Runtime::OperationCode::iload_2:
                return std::make_shared<JVM::Runtime::iload_2>();
            case JVM::Runtime::OperationCode::iload_3:
                return std::make_shared<JVM::Runtime::iload_3>();
            case JVM::Runtime::OperationCode::imul:
                return std::make_shared<JVM::Runtime::imul>();
            case JVM::Runtime::OperationCode::ineg:
                return std::make_shared<JVM::Runtime::ineg>();
            case JVM::Runtime::OperationCode::instanceof:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::instanceof>(data);
            }
            case JVM::Runtime::OperationCode::invokedynamic:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::invokedynamic>(data);
            }
            case JVM::Runtime::OperationCode::invokeinterface:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::invokeinterface>(data);
            }
            case JVM::Runtime::OperationCode::invokespecial:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::invokespecial>(data);
            }
            case JVM::Runtime::OperationCode::invokestatic:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::invokestatic>(data);
            }
            case JVM::Runtime::OperationCode::invokevirtual:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::invokevirtual>(data);
            }
            case JVM::Runtime::OperationCode::ior:
                return std::make_shared<JVM::Runtime::ior>();
            case JVM::Runtime::OperationCode::irem:
                return std::make_shared<JVM::Runtime::irem>();
            case JVM::Runtime::OperationCode::ireturn:
                return std::make_shared<JVM::Runtime::ireturn>();
            case JVM::Runtime::OperationCode::ishl:
                return std::make_shared<JVM::Runtime::ishl>();
            case JVM::Runtime::OperationCode::ishr:
                return std::make_shared<JVM::Runtime::ishr>();
            case JVM::Runtime::OperationCode::istore:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::istore>(data);
            case JVM::Runtime::OperationCode::istore_0:
                return std::make_shared<JVM::Runtime::istore_0>();
            case JVM::Runtime::OperationCode::istore_1:
                return std::make_shared<JVM::Runtime::istore_1>();
            case JVM::Runtime::OperationCode::istore_2:
                return std::make_shared<JVM::Runtime::istore_2>();
            case JVM::Runtime::OperationCode::istore_3:
                return std::make_shared<JVM::Runtime::istore_3>();
            case JVM::Runtime::OperationCode::isub:
                return std::make_shared<JVM::Runtime::isub>();
            case JVM::Runtime::OperationCode::iushr:
                return std::make_shared<JVM::Runtime::iushr>();
            case JVM::Runtime::OperationCode::ixor:
                return std::make_shared<JVM::Runtime::ixor>();
            case JVM::Runtime::OperationCode::jsr:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::jsr>(data);
            }
            case JVM::Runtime::OperationCode::jsr_w:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::jsr_w>(data);
            }
            case JVM::Runtime::OperationCode::l2d:
                return std::make_shared<JVM::Runtime::l2d>();
            case JVM::Runtime::OperationCode::l2f:
                return std::make_shared<JVM::Runtime::l2f>();
            case JVM::Runtime::OperationCode::l2i:
                return std::make_shared<JVM::Runtime::l2i>();
            case JVM::Runtime::OperationCode::ladd:
                return std::make_shared<JVM::Runtime::ladd>();
            case JVM::Runtime::OperationCode::laload:
                return std::make_shared<JVM::Runtime::laload>();
            case JVM::Runtime::OperationCode::land:
                return std::make_shared<JVM::Runtime::land>();
            case JVM::Runtime::OperationCode::lastore:
                return std::make_shared<JVM::Runtime::lastore>();
            case JVM::Runtime::OperationCode::lcmp:
                return std::make_shared<JVM::Runtime::lcmp>();
            case JVM::Runtime::OperationCode::lconst_0:
                return std::make_shared<JVM::Runtime::lconst_0>();
            case JVM::Runtime::OperationCode::lconst_1:
                return std::make_shared<JVM::Runtime::lconst_1>();
            case JVM::Runtime::OperationCode::ldc:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::ldc>(data);
            case JVM::Runtime::OperationCode::ldc_w:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::ldc_w>(data);
            }
            case JVM::Runtime::OperationCode::ldc2_w:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::ldc2_w>(data);
            }
            case JVM::Runtime::OperationCode::ldiv:
                return std::make_shared<JVM::Runtime::ldiv>();
            case JVM::Runtime::OperationCode::lload:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::lload>(data);
            case JVM::Runtime::OperationCode::lload_0:
                return std::make_shared<JVM::Runtime::lload_0>();
            case JVM::Runtime::OperationCode::lload_1:
                return std::make_shared<JVM::Runtime::lload_1>();
            case JVM::Runtime::OperationCode::lload_2:
                return std::make_shared<JVM::Runtime::lload_2>();
            case JVM::Runtime::OperationCode::lload_3:
                return std::make_shared<JVM::Runtime::lload_3>();
            case JVM::Runtime::OperationCode::lmul:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::lmul>(data);
            case JVM::Runtime::OperationCode::lneg:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::lneg>(data);
            case JVM::Runtime::OperationCode::lor:
                return std::make_shared<JVM::Runtime::lor>();
            case JVM::Runtime::OperationCode::lrem:
                return std::make_shared<JVM::Runtime::lrem>();
            case JVM::Runtime::OperationCode::lreturn:
                return std::make_shared<JVM::Runtime::lreturn>();
            case JVM::Runtime::OperationCode::lshl:
                return std::make_shared<JVM::Runtime::lshl>();
            case JVM::Runtime::OperationCode::lshr:
                return std::make_shared<JVM::Runtime::lshr>();
            case JVM::Runtime::OperationCode::lstore:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::lstore>(data);
            case JVM::Runtime::OperationCode::lstore_0:
                return std::make_shared<JVM::Runtime::lstore_0>();
            case JVM::Runtime::OperationCode::lstore_1:
                return std::make_shared<JVM::Runtime::lstore_1>();
            case JVM::Runtime::OperationCode::lstore_2:
                return std::make_shared<JVM::Runtime::lstore_2>();
            case JVM::Runtime::OperationCode::lstore_3:
                return std::make_shared<JVM::Runtime::lstore_3>();
            case JVM::Runtime::OperationCode::lsub:
                return std::make_shared<JVM::Runtime::lsub>();
            case JVM::Runtime::OperationCode::lushr:
                return std::make_shared<JVM::Runtime::lushr>();
            case JVM::Runtime::OperationCode::lxor:
                return std::make_shared<JVM::Runtime::lxor>();
            case JVM::Runtime::OperationCode::monitorenter:
                return std::make_shared<JVM::Runtime::monitorenter>();
            case JVM::Runtime::OperationCode::monitorexit:
                return std::make_shared<JVM::Runtime::monitorexit>();
            case JVM::Runtime::OperationCode::multianewarray:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::multianewarray>(data);
            }
            case JVM::Runtime::OperationCode::NEW:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::NEW>(data);
            }
            case JVM::Runtime::OperationCode::newarray:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::newarray>(data);
            case JVM::Runtime::OperationCode::nop:
                return std::make_shared<JVM::Runtime::nop>();
            case JVM::Runtime::OperationCode::pop:
                return std::make_shared<JVM::Runtime::pop>();
            case JVM::Runtime::OperationCode::pop2:
                return std::make_shared<JVM::Runtime::pop2>();
            case JVM::Runtime::OperationCode::putfield:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::putfield>(data);
            }
            case JVM::Runtime::OperationCode::putstatic:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::putstatic>(data);
            }
            case JVM::Runtime::OperationCode::ret:
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::ret>(data);
            case JVM::Runtime::OperationCode::RETURN:
                return std::make_shared<JVM::Runtime::RETURN>();
            case JVM::Runtime::OperationCode::saload:
                return std::make_shared<JVM::Runtime::saload>();
            case JVM::Runtime::OperationCode::sastore:
                return std::make_shared<JVM::Runtime::sastore>();
            case JVM::Runtime::OperationCode::sipush:
            {
                data.emplace_back(read<JVM::u1>(stream));
                data.emplace_back(read<JVM::u1>(stream));
                return std::make_shared<JVM::Runtime::sipush>(data);
            }
            case JVM::Runtime::OperationCode::swap:
                return std::make_shared<JVM::Runtime::swap>();
            case JVM::Runtime::OperationCode::tableswitch:
            {
                std::stringstream::pos_type localOffset = stream.tellg();

                // skip padding bytes
                JVM::u4 padding = firstInstructionOffset % 4;
                while(padding > 0)
                {
                    if(static_cast<JVM::u1>(read<JVM::u1>(stream)) != 0)
                    {
                        throw Compiler::RuntimeException(fmt::format("Possibly incorrect read operation occured"));
                    }

                    padding--;
                }
                
                JVM::i4 defaultValue = (static_cast<JVM::u4>(localOffset) - firstOpCodeLocalOffset) + read<JVM::i4>(stream);
                JVM::i4 lowValue = read<JVM::i4>(stream);
                JVM::i4 highValue = read<JVM::i4>(stream);

                std::vector<JVM::i4> jumpOffsets;
                const JVM::i4 jumpOffsetsCount = highValue - lowValue + 1;
                jumpOffsets.reserve(jumpOffsetsCount);
                for (size_t jumpOffsetIndex = 0; jumpOffsetIndex < jumpOffsetsCount; jumpOffsetIndex++)
                {
                    std::stringstream::pos_type currentPos = stream.tellg();
                    JVM::i4 jumpOffset = (static_cast<JVM::u4>(localOffset) - firstOpCodeLocalOffset) + read<JVM::i4>(stream);
                    jumpOffsets.emplace_back(jumpOffset);
                }

                JVM::u1 defaultByte1 = (defaultValue & 0x000000ff);
                JVM::u1 defaultByte2 = (defaultValue & 0x0000ff00) >> 8;
                JVM::u1 defaultByte3 = (defaultValue & 0x00ff0000) >> 16;
                JVM::u1 defaultByte4 = (defaultValue & 0xff000000) >> 24;

                JVM::u1 lowByte1 = (lowValue & 0x000000ff);
                JVM::u1 lowByte2 = (lowValue & 0x0000ff00) >> 8;
                JVM::u1 lowByte3 = (lowValue & 0x00ff0000) >> 16;
                JVM::u1 lowByte4 = (lowValue & 0xff000000) >> 24;

                JVM::u1 highByte1 = (highValue & 0x000000ff);
                JVM::u1 highByte2 = (highValue & 0x0000ff00) >> 8;
                JVM::u1 highByte3 = (highValue & 0x00ff0000) >> 16;
                JVM::u1 highByte4 = (highValue & 0xff000000) >> 24;

                data.emplace_back(defaultByte1);
                data.emplace_back(defaultByte2);
                data.emplace_back(defaultByte3);
                data.emplace_back(defaultByte4);

                data.emplace_back(lowByte1);
                data.emplace_back(lowByte2);
                data.emplace_back(lowByte3);
                data.emplace_back(lowByte4);

                data.emplace_back(highByte1);
                data.emplace_back(highByte2);
                data.emplace_back(highByte3);
                data.emplace_back(highByte4);

                return std::make_shared<JVM::Runtime::tableswitch>(data, jumpOffsets);
            }
            default:
            case JVM::Runtime::OperationCode::lookupswitch:
            case JVM::Runtime::OperationCode::wide:
                throw Compiler::OperationNotSupportedException(opCode);
                return nullptr;
        }
    }
}
