#ifndef SUPERJET_CODEGEN_BOOLEAN_H
#define SUPERJET_CODEGEN_BOOLEAN_H

#include "Compiler/CodeGen/Node.h"

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class Boolean : public SuperJet::Compiler::CodeGen::Node
    {
    public:
        Boolean(bool inValue) : value(inValue)
        {
        }

        virtual void dump(std::ostream& outputStream) override
        {
            outputStream << (value == true ? "true" : "false");
        }

        bool getValue() const
        {
            return value;
        }

    protected:
        bool value;
    };
}

#endif // SUPERJET_CODEGEN_BOOLEAN_H