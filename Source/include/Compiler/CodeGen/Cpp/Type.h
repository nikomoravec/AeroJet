#ifndef SUPERJET_CODE_GEN_TYPE_H
#define SUPERJET_CODE_GEN_TYPE_H

#include "Compiler/CodeGen/Node.h"
#include "Compiler/Exceptions/RuntimeException.h"
#include <string>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class Type : public SuperJet::Compiler::CodeGen::Node
    {
    public:
        enum class Flags : uint8_t
        {
            NONE = 0x0000,
            CONST = 0x0001,
            STATIC = 0x0002,
            POINTER = 0x0004,
            CONST_POINTER = 0x0008
        };


        Type(const std::string& inName, Flags inFlags = Flags::NONE) : name(inName)
        {
            if (name.empty())
            {
                throw SuperJet::Compiler::RuntimeException("Empty name is not allowed!");
            }

            if (isPointer() && isConstPointer())
            {
                throw SuperJet::Compiler::RuntimeException("Empty name is not allowed!");
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
            static_cast<uint8_t>(flags) & static_cast<uint8_t>(Flags::STATIC);
        }

        bool isConst() const
        {
            static_cast<uint8_t>(flags) & static_cast<uint8_t>(Flags::CONST);
        }

        bool isPointer() const
        {
            static_cast<uint8_t>(flags) & static_cast<uint8_t>(Flags::POINTER);
        }

        bool isConstPointer() const
        {
            static_cast<uint8_t>(flags) & static_cast<uint8_t>(Flags::CONST_POINTER);
        }

        bool hasNoFlags() const
        {
            static_cast<uint8_t>(flags) & static_cast<uint8_t>(Flags::NONE) && !isStatic() && !isConst() && !isPointer() && !isConstPointer();
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
        }

    protected:
        std::string name;
        Flags flags;
    };
}

#endif // SUPERJET_CODE_GEN_TYPE_H