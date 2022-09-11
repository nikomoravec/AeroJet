#ifndef SUPERJET_CODEGEN_CHARACTER_H
#define SUPERJET_CODEGEN_CHARACTER_H

#include "Java/Types.h"
#include "Compiler/CodeGen/Node.h"

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class Character : public SuperJet::Compiler::CodeGen::Node
    {
    public:
        Character() : value(0x00)
        {
        }

        Character(int32_t inValue) : value(inValue)
        {
        }

        virtual void dump(std::ostream& outputStream) override
        {
            outputStream << value;
        }

    protected:
        int32_t value;
    };
}

#endif // SUPERJET_CODEGEN_CHARACTER_H