#include "Java/Archive/Archive.h"
#include "Java/Archive/ClassInfo.h"
#include "Java/Archive/ConstantPoolEntry.h"
#include "Java/Archive/MethodInfo.h"
#include "Java/JVM/Runtime/Instruction.h"
#include "Java/JVM/Runtime/Operations.h"
#include "Compiler/Exceptions/NotSupportedException.h"
#include "Compiler/Exceptions/OperationNotSupportedException.h"
#include "Compiler/Exceptions/UnknownConstantPoolTagException.h"
#include "fmt/format.h"
#include "spdlog/spdlog.h"

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

namespace
{
    unsigned int allign(unsigned int n)
    {
        return (n + 3) & ~3;
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

    template<typename T>
    static inline void write(T object, std::ostream& stream)
    {
        stream.write((char*)&object, sizeof(object));
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
        const JVM::u4 infoOffset = static_cast<JVM::u4>(stream.tellg());

        const JVM::u2 readAttributeNameIndex = read<JVM::u2>(stream);
        const JVM::u4 readAttributeInfoSize = read<JVM::u4>(stream);

        std::vector<JVM::u1> attributeInfo;
        attributeInfo.reserve(readAttributeInfoSize);
        for (int32_t attributeInfoIndex = 0; attributeInfoIndex < readAttributeInfoSize; attributeInfoIndex++)
        {
            attributeInfo.emplace_back(read<JVM::u1>(stream));
        }

        return AttributeInfo{readAttributeNameIndex, attributeInfo, infoOffset};
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
    SuperJet::Java::JVM::Runtime::Instruction read(std::istream& stream, JVM::u4 firstInstructionOffset, JVM::u4 firstOpCodeLocalOffset)
    {
        std::stringstream data;

        const SuperJet::Java::JVM::Runtime::OperationCode opCode = static_cast<SuperJet::Java::JVM::Runtime::OperationCode>(read<JVM::u1>(stream));        
        // spdlog::info(fmt::format("opcode: {:#04x}, offset: {:#08x}", static_cast<JVM::u1>(opCode), firstInstructionOffset + static_cast<JVM::u8>(stream.tellg())));

        switch (opCode)
        {
            case JVM::Runtime::OperationCode::aaload:
            case JVM::Runtime::OperationCode::aastore:
            case JVM::Runtime::OperationCode::aconst_null:
            case JVM::Runtime::OperationCode::areturn:
            case JVM::Runtime::OperationCode::arraylength:
            case JVM::Runtime::OperationCode::athrow:
            case JVM::Runtime::OperationCode::baload:
            case JVM::Runtime::OperationCode::bastore:
            case JVM::Runtime::OperationCode::caload:
            case JVM::Runtime::OperationCode::castore:
            case JVM::Runtime::OperationCode::d2f:
            case JVM::Runtime::OperationCode::d2i:
            case JVM::Runtime::OperationCode::d2l:
            case JVM::Runtime::OperationCode::dadd:
            case JVM::Runtime::OperationCode::daload:
            case JVM::Runtime::OperationCode::dastore:
            case JVM::Runtime::OperationCode::dcmpg:
            case JVM::Runtime::OperationCode::dcmpl:
            case JVM::Runtime::OperationCode::ddiv:
            case JVM::Runtime::OperationCode::dmul:
            case JVM::Runtime::OperationCode::dneg:
            case JVM::Runtime::OperationCode::drem:
            case JVM::Runtime::OperationCode::dreturn:
            case JVM::Runtime::OperationCode::dsub:
            case JVM::Runtime::OperationCode::dup:
            case JVM::Runtime::OperationCode::dup_x1:
            case JVM::Runtime::OperationCode::dup_x2:
            case JVM::Runtime::OperationCode::dup2:
            case JVM::Runtime::OperationCode::dup2_x1:
            case JVM::Runtime::OperationCode::dup2_x2:
            case JVM::Runtime::OperationCode::f2i:
            case JVM::Runtime::OperationCode::f2d:
            case JVM::Runtime::OperationCode::f2l:
            case JVM::Runtime::OperationCode::fadd:
            case JVM::Runtime::OperationCode::faload:
            case JVM::Runtime::OperationCode::fastore:
            case JVM::Runtime::OperationCode::fcmpg:
            case JVM::Runtime::OperationCode::fcmpl:
            case JVM::Runtime::OperationCode::fdiv:
            case JVM::Runtime::OperationCode::fmul:
            case JVM::Runtime::OperationCode::fneg:
            case JVM::Runtime::OperationCode::frem:
            case JVM::Runtime::OperationCode::freturn:
            case JVM::Runtime::OperationCode::fsub:
            case JVM::Runtime::OperationCode::i2b:
            case JVM::Runtime::OperationCode::i2c:
            case JVM::Runtime::OperationCode::i2d:
            case JVM::Runtime::OperationCode::i2f:
            case JVM::Runtime::OperationCode::i2l:
            case JVM::Runtime::OperationCode::i2s:
            case JVM::Runtime::OperationCode::iadd:
            case JVM::Runtime::OperationCode::iaload:
            case JVM::Runtime::OperationCode::iand:
            case JVM::Runtime::OperationCode::iastore:
            case JVM::Runtime::OperationCode::aload_0:
            case JVM::Runtime::OperationCode::astore_0:
            case JVM::Runtime::OperationCode::dload_0:
            case JVM::Runtime::OperationCode::dstore_0:
            case JVM::Runtime::OperationCode::fconst_0:
            case JVM::Runtime::OperationCode::fload_0:
            case JVM::Runtime::OperationCode::fstore_0:
            case JVM::Runtime::OperationCode::aload_1:
            case JVM::Runtime::OperationCode::astore_1:
            case JVM::Runtime::OperationCode::dload_1:
            case JVM::Runtime::OperationCode::dstore_1:
            case JVM::Runtime::OperationCode::fconst_1:
            case JVM::Runtime::OperationCode::fload_1:
            case JVM::Runtime::OperationCode::fstore_1:
            case JVM::Runtime::OperationCode::aload_2:
            case JVM::Runtime::OperationCode::astore_2:
            case JVM::Runtime::OperationCode::dload_2:
            case JVM::Runtime::OperationCode::dstore_2:
            case JVM::Runtime::OperationCode::fconst_2:
            case JVM::Runtime::OperationCode::fload_2:
            case JVM::Runtime::OperationCode::fstore_2:
            case JVM::Runtime::OperationCode::iconst_m1:
            case JVM::Runtime::OperationCode::aload_3:
            case JVM::Runtime::OperationCode::astore_3:
            case JVM::Runtime::OperationCode::dconst_0:
            case JVM::Runtime::OperationCode::dconst_1:
            case JVM::Runtime::OperationCode::dload_3:
            case JVM::Runtime::OperationCode::dstore_3:
            case JVM::Runtime::OperationCode::fload_3:
            case JVM::Runtime::OperationCode::fstore_3:
            case JVM::Runtime::OperationCode::iconst_0:
            case JVM::Runtime::OperationCode::iconst_1:
            case JVM::Runtime::OperationCode::iconst_2:
            case JVM::Runtime::OperationCode::iconst_3:
            case JVM::Runtime::OperationCode::iconst_4:
            case JVM::Runtime::OperationCode::iconst_5:
            case JVM::Runtime::OperationCode::idiv:
            case JVM::Runtime::OperationCode::ior:
            case JVM::Runtime::OperationCode::iload_0:
            case JVM::Runtime::OperationCode::iload_1:
            case JVM::Runtime::OperationCode::iload_2:
            case JVM::Runtime::OperationCode::iload_3:
            case JVM::Runtime::OperationCode::imul:
            case JVM::Runtime::OperationCode::ineg:
            case JVM::Runtime::OperationCode::irem:
            case JVM::Runtime::OperationCode::ireturn:
            case JVM::Runtime::OperationCode::ishl:
            case JVM::Runtime::OperationCode::ishr:
            case JVM::Runtime::OperationCode::istore_0:
            case JVM::Runtime::OperationCode::istore_1:
            case JVM::Runtime::OperationCode::istore_2:
            case JVM::Runtime::OperationCode::istore_3:
            case JVM::Runtime::OperationCode::isub:
            case JVM::Runtime::OperationCode::iushr:
            case JVM::Runtime::OperationCode::ixor:
            case JVM::Runtime::OperationCode::l2d:
            case JVM::Runtime::OperationCode::l2f:
            case JVM::Runtime::OperationCode::l2i:
            case JVM::Runtime::OperationCode::ladd:
            case JVM::Runtime::OperationCode::laload:
            case JVM::Runtime::OperationCode::land:
            case JVM::Runtime::OperationCode::lastore:
            case JVM::Runtime::OperationCode::lcmp:
            case JVM::Runtime::OperationCode::lconst_0:
            case JVM::Runtime::OperationCode::lconst_1:
            case JVM::Runtime::OperationCode::ldiv:
            case JVM::Runtime::OperationCode::lload_0:
            case JVM::Runtime::OperationCode::lload_1:
            case JVM::Runtime::OperationCode::lload_2:
            case JVM::Runtime::OperationCode::lload_3:
            case JVM::Runtime::OperationCode::lor:
            case JVM::Runtime::OperationCode::lrem:
            case JVM::Runtime::OperationCode::lreturn:
            case JVM::Runtime::OperationCode::lshl:
            case JVM::Runtime::OperationCode::lshr:
            case JVM::Runtime::OperationCode::lstore_0:
            case JVM::Runtime::OperationCode::lstore_1:
            case JVM::Runtime::OperationCode::lstore_2:
            case JVM::Runtime::OperationCode::lstore_3:
            case JVM::Runtime::OperationCode::lsub:
            case JVM::Runtime::OperationCode::lushr:
            case JVM::Runtime::OperationCode::lxor:
            case JVM::Runtime::OperationCode::monitorenter:
            case JVM::Runtime::OperationCode::monitorexit:
            case JVM::Runtime::OperationCode::nop:
            case JVM::Runtime::OperationCode::pop:
            case JVM::Runtime::OperationCode::pop2:
            case JVM::Runtime::OperationCode::RETURN:
            case JVM::Runtime::OperationCode::saload:
            case JVM::Runtime::OperationCode::sastore:
            case JVM::Runtime::OperationCode::swap:
                break;
            case JVM::Runtime::OperationCode::aload:
            case JVM::Runtime::OperationCode::astore:
            case JVM::Runtime::OperationCode::bipush:
            case JVM::Runtime::OperationCode::dload:
            case JVM::Runtime::OperationCode::dstore:
            case JVM::Runtime::OperationCode::fload:
            case JVM::Runtime::OperationCode::fstore:
            case JVM::Runtime::OperationCode::iload:
            case JVM::Runtime::OperationCode::istore:
            case JVM::Runtime::OperationCode::ldc:
            case JVM::Runtime::OperationCode::lload:
            case JVM::Runtime::OperationCode::lmul:
            case JVM::Runtime::OperationCode::lneg:
            case JVM::Runtime::OperationCode::lstore:
            case JVM::Runtime::OperationCode::newarray:
            case JVM::Runtime::OperationCode::ret:
                write(read<JVM::u1>(stream), data);
                break;
            case JVM::Runtime::OperationCode::anewarray:
            case JVM::Runtime::OperationCode::checkcast:
            case JVM::Runtime::OperationCode::getfield:
            case JVM::Runtime::OperationCode::getstatic:
            case JVM::Runtime::OperationCode::GOTO:
            case JVM::Runtime::OperationCode::if_acmpeq:
            case JVM::Runtime::OperationCode::if_acmpne:
            case JVM::Runtime::OperationCode::if_icmpeq:
            case JVM::Runtime::OperationCode::if_icmpne:
            case JVM::Runtime::OperationCode::if_icmpge:
            case JVM::Runtime::OperationCode::if_icmpgt:
            case JVM::Runtime::OperationCode::if_icmple:
            case JVM::Runtime::OperationCode::ifeq:
            case JVM::Runtime::OperationCode::ifne:
            case JVM::Runtime::OperationCode::iflt:
            case JVM::Runtime::OperationCode::ifge:
            case JVM::Runtime::OperationCode::ifgt:
            case JVM::Runtime::OperationCode::ifle:
            case JVM::Runtime::OperationCode::ifnonnull:
            case JVM::Runtime::OperationCode::ifnull:
            case JVM::Runtime::OperationCode::if_icmplt:
            case JVM::Runtime::OperationCode::iinc:
            case JVM::Runtime::OperationCode::instanceof:
            case JVM::Runtime::OperationCode::invokespecial:
            case JVM::Runtime::OperationCode::invokestatic:
            case JVM::Runtime::OperationCode::invokevirtual:
            case JVM::Runtime::OperationCode::jsr:
            case JVM::Runtime::OperationCode::ldc_w:
            case JVM::Runtime::OperationCode::ldc2_w:
            case JVM::Runtime::OperationCode::NEW:
            case JVM::Runtime::OperationCode::putfield:
            case JVM::Runtime::OperationCode::putstatic:
            case JVM::Runtime::OperationCode::sipush:
            {
                write(read<JVM::u1>(stream), data);
                write(read<JVM::u1>(stream), data);
                break;
            }
            case JVM::Runtime::OperationCode::multianewarray:
            {
                write(read<JVM::u1>(stream), data);
                write(read<JVM::u1>(stream), data);
                write(read<JVM::u1>(stream), data);
                break;
            }
            case JVM::Runtime::OperationCode::goto_w:
            case JVM::Runtime::OperationCode::invokedynamic:
            case JVM::Runtime::OperationCode::invokeinterface:
            case JVM::Runtime::OperationCode::jsr_w:
            {
                write(read<JVM::u1>(stream), data);
                write(read<JVM::u1>(stream), data);
                write(read<JVM::u1>(stream), data);
                write(read<JVM::u1>(stream), data);
                break;
            }
            case JVM::Runtime::OperationCode::tableswitch:
            {
                const JVM::u4 localOffset = (static_cast<JVM::u4>(stream.tellg()) - 9);

                // skip padding bytes
                const JVM::u4 padding = allign(localOffset + 1) - localOffset;
                {
                    JVM::u4 paddingIndex = padding;
                    while((paddingIndex - 1) > 0)
                    {                        
                        read<JVM::u1>(stream);
                        paddingIndex--;
                    }
                }

                
                JVM::i4 defaultValue = localOffset + read<JVM::i4>(stream);
                JVM::i4 lowValue = read<JVM::i4>(stream);
                JVM::i4 highValue = read<JVM::i4>(stream);

                std::vector<JVM::i4> jumpOffsets;
                const JVM::i4 jumpOffsetsCount = highValue - lowValue + 1;
                jumpOffsets.reserve(jumpOffsetsCount);
                for (size_t jumpOffsetIndex = 0; jumpOffsetIndex < jumpOffsetsCount; jumpOffsetIndex++)
                {
                    std::stringstream::pos_type currentPos = stream.tellg();
                    JVM::i4 jumpOffset = localOffset + read<JVM::i4>(stream);
                    jumpOffsets.emplace_back(jumpOffset);
                }

                write(defaultValue, data);
                write(lowValue, data);
                write(highValue, data);

                break;
            }
            case JVM::Runtime::OperationCode::lookupswitch:
            {
                const JVM::u4 localOffset = (static_cast<JVM::u4>(stream.tellg()) - 9);

                // skip padding bytes
                const JVM::u4 padding = allign(localOffset + 1) - localOffset;
                {
                    JVM::u4 paddingIndex = padding;
                    while((paddingIndex - 1) > 0)
                    {                        
                        read<JVM::u1>(stream);
                        paddingIndex--;
                    }
                }

                const JVM::i4 defaultValue = localOffset + read<JVM::i4>(stream);
                write(defaultValue, data);

                const JVM::i4 npairsCount = read<JVM::i4>(stream);
                write(npairsCount, data);

                for (JVM::i4 npairIndex = 0; npairIndex < npairsCount; npairIndex++)
                {
                    write(read<JVM::i4>(stream), data);
                    write(read<JVM::i4>(stream), data);
                }

                break;
            }
            case JVM::Runtime::OperationCode::wide:
            {
                const JVM::Runtime::OperationCode nextOpCode = static_cast<JVM::Runtime::OperationCode>(read<JVM::u1>(stream));
                write(static_cast<JVM::u1>(nextOpCode), data);

                switch (nextOpCode)
                {
                    case JVM::Runtime::OperationCode::iload:
                    case JVM::Runtime::OperationCode::fload:
                    case JVM::Runtime::OperationCode::aload:
                    case JVM::Runtime::OperationCode::lload:
                    case JVM::Runtime::OperationCode::dload:
                    case JVM::Runtime::OperationCode::istore:
                    case JVM::Runtime::OperationCode::fstore:
                    case JVM::Runtime::OperationCode::astore:
                    case JVM::Runtime::OperationCode::lstore:
                    case JVM::Runtime::OperationCode::dstore:
                    case JVM::Runtime::OperationCode::ret:
                    {
                        write(read<JVM::u1>(stream), data);
                        write(read<JVM::u1>(stream), data);
                        break;
                    }
                    case JVM::Runtime::OperationCode::iinc:
                    {
                        write(read<JVM::u1>(stream), data);
                        write(read<JVM::u1>(stream), data);
                        write(read<JVM::u1>(stream), data);
                        write(read<JVM::u1>(stream), data);
                        break;
                    }
                    default:
                        throw Compiler::RuntimeException(fmt::format("Unexpected OpCode ({:#04x}) after 'wide'!"));
                }
            break;
            }
            default:
                throw Compiler::OperationNotSupportedException(opCode);
        }

        size_t dataSize = data.tellp();
        std::vector<JVM::u1> dataVector{};
        dataVector.reserve(dataSize);
        while (dataSize > 0)
        {
            dataVector.emplace_back(read<JVM::u1>(data));
            dataSize--;
        }
            
        return SuperJet::Java::JVM::Runtime::Instruction(opCode, std::move(dataVector));
    }
}
