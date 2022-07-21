#ifndef SUPERJET_CODE_GEN_TEMPLATE_H
#define SUPERJET_CODE_GEN_TEMPLATE_H

#include "Compiler/CodeGen/Node.h"
#include "Compiler/CodeGen/Cpp/TypeName.h"

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class Template : public Compiler::CodeGen::Node
    {
    public:
        Template() = default;

        virtual void dump(std::ostream& outputStream) override
        {
            outputStream << "template<";
            Compiler::CodeGen::Node::dump(outputStream);
            outputStream << ">";
        }

        void addTypeName(std::shared_ptr<Compiler::CodeGen::Cpp::TypeName> typeName)
        {
            addNode(typeName);
        }
    };
}

#endif // SUPERJET_CODE_GEN_TEMPLATE_H