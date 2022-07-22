#ifndef SUPERJET_CLASS_H
#define SUPERJET_CLASS_H

#include "Compiler/Compiler.h"
#include "Compiler/CodeGen/Node.h"
#include "Compiler/CodeGen/Cpp/Namespace.h"
#include "Compiler/CodeGen/Cpp/Variable.h"
#include "Compiler/CodeGen/Cpp/Template.h"
#include "fmt/format.h"
#include <string>
#include <optional>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class Class : public Compiler::CodeGen::Node
    {
        friend void SuperJet::Compiler::CodeGen::Cpp::Namespace::addClass(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Class>);

    public:
        Class(const std::string& className) : name(className), templade(nullptr)
        {
        }

        bool hasTemplate() const
        {
            return templade != nullptr;
        }

        bool hasNamespace() const
        {
            return ns.has_value();
        }

        const Namespace& getNamespace() const
        {
            return ns.value();
        }

        std::string getName() const
        {
            return name;
        }

        std::string getFullName() const
        {
            if (hasNamespace())
            {
                return ns->asString() + Namespace::CPP_NAMESPACE_DELIMITER + name;
            }

            return name;
        }

        void addVariable(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Variable> var)
        {
            addNode(var);
        }

        void setTemplate(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Template> templ)
        {
            templade = templ;
        }

        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Template> getTemplate()
        {
            return templade;
        }

        virtual void dump(std::ostream& outputStream) override
        {
            if (hasTemplate())
            {
                templade->dump(outputStream);
                outputStream << '\n';
            }

            outputStream << fmt::format("struct {}", getName());
            outputStream << "\n{\n";
            for (const auto& child : childrens)
            {
                child->dump(outputStream);
            }
            outputStream << "\n};";
        }

    protected:
        std::optional<SuperJet::Compiler::CodeGen::Cpp::Namespace> ns;
        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Template> templade;
        
        std::string name;
    };
}

#endif //SUPERJET_CLASS_H
