#ifndef SUPERJET_CODEGEN_TEMPLATE_H
#define SUPERJET_CODEGEN_TEMPLATE_H

#include "Compiler/CodeGen/Node.h"
#include "Compiler/CodeGen/Cpp/TypeName.h"

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class Template : public Compiler::CodeGen::Node
    {
    public:
        virtual void dump(std::ostream& outputStream) override
        {
            outputStream << "template<";

            for (int32_t childIndex = 0; childIndex < childrens.size(); childIndex++)
            {
                const std::shared_ptr<SuperJet::Compiler::CodeGen::Node>& child = childrens[childIndex];
                child->dump(outputStream);

                if (childIndex != childrens.size() - 1)
                {
                    outputStream << ',';
                }
            }

            outputStream << ">";
        }

        void addTypeName(std::shared_ptr<Compiler::CodeGen::Cpp::TypeName> typeName)
        {
            addNode(typeName);
        }
    };
}

#endif // SUPERJET_CODEGEN_TEMPLATE_H