#ifndef SUPERJET_CPPCODEGEN_FORWARD_DECLARATION_H
#define SUPERJET_CPPCODEGEN_FORWARD_DECLARATION_H

#include "Compiler/CodeGen/Node.h"
#include "Compiler/CodeGen/Cpp/Class.h"
#include "Compiler/Exceptions/RuntimeException.h"

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class ForwardDeclaration : public SuperJet::Compiler::CodeGen::Node
    {
    public:
        ForwardDeclaration(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Class> c) : clazz(c)
        {
            if (clazz == nullptr)
            {
                throw(Compiler::RuntimeException("Class can not be null!"));
            }
        }

        void dump(std::ostream& outputStream) override
        {
            outputStream << fmt::format("struct {};", clazz->getName());
        }

    protected:
        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Class> clazz;
    };
}


#endif // SUPERJET_CPPCODEGEN_FORWARD_DECLARATION_H