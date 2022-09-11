#ifndef SUPERJET_CODEGEN_ASSIGN_OPERATION_H
#define SUPERJET_CODEGEN_ASSIGN_OPERATION_H

#include "Compiler/CodeGen/Node.h"
#include "Compiler/CodeGen/Cpp/Variable.h"
#include "fmt/format.h"

#include <memory>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class VarialeAssignOperation : public SuperJet::Compiler::CodeGen::Node
    {
    public:
        VarialeAssignOperation(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Variable> inLhs, std::shared_ptr<SuperJet::Compiler::CodeGen::Node> inRhs) : lhs(inLhs), rhs(inRhs)
        {

        }

        virtual void dump(std::ostream& outputStream) override
        {
            outputStream << fmt::format("{} = ", lhs->getName());
            rhs->dump(outputStream);
            outputStream << ";\n";
        }

    protected:
        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Variable> lhs;
        std::shared_ptr<SuperJet::Compiler::CodeGen::Node> rhs;
    };
}

#endif // SUPERJET_CODEGEN_ASSIGN_OPERATION_H