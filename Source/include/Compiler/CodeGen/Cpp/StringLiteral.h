#ifndef SUPERJET_CODEGEN_STRINGLITERAL_H
#define SUPERJET_CODEGEN_STRINGLITERAL_H

#include "Compiler/CodeGen/Node.h"
#include "fmt/format.h"
#include <string>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class StringLiteral : public SuperJet::Compiler::CodeGen::Node
    {
    public:
        StringLiteral(const std::string& inValue) : value(inValue)
        {
        }

        virtual void dump(std::ostream& outputStream) override
        {
            outputStream << fmt::format("\"{}\"", value);
        }

    protected:
        std::string value;
    };
}

#endif // SUPERJET_CODEGEN_STRINGLITERAL_H