#include "Compiler/CodeGen/Cpp/Namespace.h"
#include "Compiler/CodeGen/Cpp/ForwardDeclaration.h"
#include "Compiler/CodeGen/Cpp/Class.h"

namespace SuperJet::Compiler::CodeGen::Cpp
{
    void Namespace::addForwardDeclaration(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::ForwardDeclaration> forwardDeclaration)
    {
        childrens.emplace_back(forwardDeclaration);
    }

    void Namespace::addClass(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Class> clazz)
    {
        clazz->ns = *this;
        childrens.emplace_back(clazz);
    }
}