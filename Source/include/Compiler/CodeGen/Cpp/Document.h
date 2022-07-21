#ifndef SUPERJET_CPPCODEGEN_DOCUMENT_H
#define SUPERJET_CPPCODEGEN_DOCUMENT_H

#include "Compiler/CodeGen/Node.h"
#include "Compiler/CodeGen/Cpp/Namespace.h"

#include <filesystem>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class Document : public SuperJet::Compiler::CodeGen::Node
    {
    public:
        void addNameSpace(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Namespace> ns)
        {
            childrens.emplace_back(ns);
        }

        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Namespace> addNameSpace(const std::filesystem::path& package)
        {
            std::shared_ptr ns = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Namespace>(package);
            childrens.emplace_back(ns);
            return ns;
        }

        void addForwardDeclaration(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::ForwardDeclaration> forwardDeclaration)
        {
             childrens.emplace_back(forwardDeclaration);
        }

        void addClass(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Class> clazz)
        {
            childrens.emplace_back(clazz);
        }

        void dump(std::ostream& outputStream)
        {
            for (const auto& children : childrens)
            {
                children->dump(outputStream);
                outputStream << '\n';
            }
        }
    };
}

#endif