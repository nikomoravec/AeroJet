#ifndef SUPERJET_CODEGENERATOR_H
#define SUPERJET_CODEGENERATOR_H

namespace SuperJet::Compiler::CodeGen
{
    template<typename T = void>
    class CodeGenerator
    {
    public:
        T generate();
    };
}

#endif //SUPERJET_CODEGENERATOR_H
