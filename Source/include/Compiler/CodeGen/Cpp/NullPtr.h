#ifndef SUPERJET_CODEGEN_NULLPTR_H
#define SUPERJET_CODEGEN_NULLPTR_H

#include "Compiler/CodeGen/Node.h"

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class NullPtr : public SuperJet::Compiler::CodeGen::Node
    {
    public:
        virtual void dump(std::ostream& outputStream) override
        {
            outputStream << "nullptr";
        }
    };
}

#endif // SUPERJET_CODEGEN_NULLPTR_H