#ifndef SUPERJET_CODEGEN_FUNCTION_H
#define SUPERJET_CODEGEN_FUNCTION_H

#include "Compiler/CodeGen/Node.h"
#include "Compiler/CodeGen/Cpp/Type.h"

#include <memory>
#include <string>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class FunctionArgument : public SuperJet::Compiler::CodeGen::Node
    {
    public:
        FunctionArgument(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> inType, const std::string& inName) : type(inType), name(inName)
        {
        }

        virtual void dump(std::ostream& outputStream) override
        {
            type->dump(outputStream);
            outputStream << " " << name;
        }

    protected:
        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> type;
        std::string name;
    };

    class Functuon : public SuperJet::Compiler::CodeGen::Node
    {
    public:
        Functuon(const std::string& inName, std::vector<std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::FunctionArgument>> inArguments = {}, std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> inReturnType = nullptr)
        : name(inName), arguments(inArguments), returnType(inReturnType)
        {
            if (returnType == nullptr)
            {
                static const std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> defaultReturnType = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Type>("void");
                returnType = defaultReturnType;
            }
        }

        const std::string& getName() const
        {
            return name;
        }

        const std::vector<std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::FunctionArgument>>& getArguments() const
        {
            return arguments;
        }

        const std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> getReturnType() const
        {
            return returnType;
        }

        virtual void dump(std::ostream& outputStream) override
        {

            returnType->dump(outputStream);

            outputStream << ' ';
            outputStream << name;

            outputStream << '(';
            for (const std::shared_ptr<SuperJet::Compiler::CodeGen::Node> argument : arguments)
            {
                argument->dump(outputStream);
            }
            outputStream << ")\n";

            outputStream << "{\n";
            SuperJet::Compiler::CodeGen::Node::dump(outputStream);
            outputStream << "}\n";
        }

    protected:
        std::string name;
        std::vector<std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::FunctionArgument>> arguments;
        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Type> returnType;
    };
}

#endif // SUPERJET_CODEGEN_FUNCTION_H