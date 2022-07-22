#ifndef SUPERJET_CODEGEN_TYPENAME_H
#define SUPERJET_CODEGEN_TYPENAME_H

#include "Compiler/CodeGen/Node.h"
#include "Compiler/CodeGen/Cpp/Type.h"
#include "fmt/format.h"

#include <string>
#include <memory>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class TypeName : public Compiler::CodeGen::Node
    {
    public:
        TypeName(const std::string& inName, std::shared_ptr<Compiler::CodeGen::Cpp::Type> defaultType = nullptr) : name(inName), defaultValue(defaultType)
        {
        }

        virtual void dump(std::ostream& outputStream) override
        {
            outputStream << fmt::format("typename {}", name);
            if (hasDefaultValueType())
            {
                outputStream << fmt::format(" = {}", defaultValue->getName());
            }
        }

        const std::string& getName() const
        {
            return name;
        }

        bool hasDefaultValueType() const
        {
            return defaultValue != nullptr;
        }

    protected:
        std::string name;
        std::shared_ptr<Compiler::CodeGen::Cpp::Type> defaultValue;
    };
}

#endif // SUPERJET_CODEGEN_TYPENAME_H