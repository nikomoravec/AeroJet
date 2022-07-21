#ifndef SUPERJET_CODEGEN_VARIABLE_H
#define SUPERJET_CODEGEN_VARIABLE_H

#include "Compiler/CodeGen/Node.h"
#include "Compiler/CodeGen/Cpp/Type.h"
#include "Compiler/CodeGen/Cpp/Value.h"
#include "Compiler/Exceptions/RuntimeException.h"

#include "fmt/format.h"

#include <memory>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class Variable : public SuperJet::Compiler::CodeGen::Node
    {
    public:
        Variable(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> inType, const std::string& inName, std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Value> inValue = nullptr) : type(inType), name(inName), value(inValue)
        {
            if ((type->isReference() || type->isConst() || type->isConstPointer()) && value == nullptr)
            {
                throw SuperJet::Compiler::RuntimeException("Value can not be empty!");
            }
        }

        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> getType() const
        {
            return type;
        }

        const std::string& getName() const
        {
            return name;
        }

        virtual void dump(std::ostream& outputStream) override
        {
            type->dump(outputStream);
            outputStream << fmt::format(" {}", name);

            if (value != nullptr)
            {
                outputStream << fmt::format(" = ");
                value->dump(outputStream);
            }


            outputStream << ';';
        }


    protected:
        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> type;
        std::string name;
        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Value> value;
    };
}

#endif // SUPERJET_CODEGEN_VARIABLE_H