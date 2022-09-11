#ifndef SUPERJET_CODEGEN_CHARACTER_H
#define SUPERJET_CODEGEN_CHARACTER_H

#include "Java/Types.h"
#include "Compiler/CodeGen/Node.h"

#include <string>
#include <optional>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class Character : public SuperJet::Compiler::CodeGen::Node
    {
    public:
        Character() : value(std::nullopt)
        {
        }

        Character(int32_t inValue) : value({static_cast<wchar_t>(inValue)})
        {
        }

        Character(wchar_t inValue) : value({inValue})
        {
        }

        virtual void dump(std::ostream& outputStream) override
        {
            if (!value.has_value())
            {
                outputStream << static_cast<wchar_t>(0x00);
            }
            else
            {
                outputStream << "L'" << value.value() << "'";
            }
        }

    protected:
        std::optional<wchar_t> value;
    };
}

#endif // SUPERJET_CODEGEN_CHARACTER_H