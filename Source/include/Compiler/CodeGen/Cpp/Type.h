#ifndef SUPERJET_CODEGEN_TYPE_H
#define SUPERJET_CODEGEN_TYPE_H

#include "Compiler/CodeGen/Node.h"
#include "Compiler/Exceptions/RuntimeException.h"
#include "fmt/format.h"
#include <string>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class Type : public SuperJet::Compiler::CodeGen::Node
    {
    public:
        enum class Flags : uint16_t
        {
            NONE = 0x0000,
            CONST = 0x0001,
            STATIC = 0x0002,
            POINTER = 0x0004,
            CONST_POINTER = 0x0008,
            REFERENCE = 0x0010
        };


        Type(const std::string& inName, Flags inFlags = Flags::NONE) : name(inName), flags(inFlags)
        {
            if (name.empty())
            {
                throw SuperJet::Compiler::RuntimeException("Empty name is not allowed!");
            }

            if (isPointer() && isConstPointer())
            {
                throw SuperJet::Compiler::RuntimeException("Conflicting type flags: POINTER and CONST_POINTER");
            }

            if (isReference() && isPointer())
            {
                throw SuperJet::Compiler::RuntimeException("Conflicting type flags: REFERENCE and POINTER");
            }

            if (isReference() && isConstPointer())
            {
                throw SuperJet::Compiler::RuntimeException("Conflicting type flags: REFERENCE and CONST_POINTER");
            }
        }

        const std::string& getName() const
        {
            return name;
        }

        Flags getFlags() const
        {
            return flags;
        }

        bool isStatic() const
        {
            return static_cast<uint8_t>(flags) & static_cast<uint8_t>(Flags::STATIC);
        }

        bool isConst() const
        {
            return static_cast<uint8_t>(flags) & static_cast<uint8_t>(Flags::CONST);
        }

        bool isPointer() const
        {
            return static_cast<uint8_t>(flags) & static_cast<uint8_t>(Flags::POINTER);
        }

        bool isConstPointer() const
        {
            return static_cast<uint8_t>(flags) & static_cast<uint8_t>(Flags::CONST_POINTER);
        }

        bool isReference() const
        {
            return static_cast<uint8_t>(flags) & static_cast<uint8_t>(Flags::REFERENCE);
        }

        bool hasNoFlags() const
        {
            return static_cast<uint8_t>(flags) & static_cast<uint8_t>(Flags::NONE) && !isStatic() && !isConst() && !isPointer() && !isConstPointer() && !isReference();
        }

        void dump(std::ostream& outputStream)
        {
            if (isStatic())
            {
                outputStream << "static ";
            }

            if (isConst())
            {
                outputStream << "const ";
            }

            outputStream << getName();

            if (isPointer())
            {
                outputStream << "*";
            }

            if (isConstPointer())
            {
                outputStream << " const*";
            }

            if (isReference())
            {
                outputStream << "&";
            }
        }

    protected:
        std::string name;
        Flags flags;
    };

    inline Type::Flags operator| (Type::Flags lhs, Type::Flags rhs)
    {
        using T = std::underlying_type_t <Type::Flags>;
        return static_cast<Type::Flags>(static_cast<T>(lhs) | static_cast<T>(rhs));
    }
}


#endif // SUPERJET_CODEGEN_TYPE_H