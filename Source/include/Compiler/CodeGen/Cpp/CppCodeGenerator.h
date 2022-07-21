#ifndef SUPERJET_CPPCODEGENERATOR_H
#define SUPERJET_CPPCODEGENERATOR_H

#include "Compiler/CodeGen/CodeGenerator.h"

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class CppCodeGenerator : public CodeGenerator<>
    {
    public:
        CppCodeGenerator() : tabMultiplier(0)
        {
        }

        

    protected:
        int tabMultiplier;
    };
}

#endif //SUPERJET_CPPCODEGENERATOR_H
