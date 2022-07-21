#ifndef SUPERJET_CLASS_H
#define SUPERJET_CLASS_H

#include "Compiler/Compiler.h"
#include "Compiler/CodeGen/Node.h"
#include "Compiler/CodeGen/Cpp/Namespace.h"
#include "Compiler/CodeGen/Cpp/Field.h"
#include "Java/Archive/ClassInfo.h"
#include "fmt/format.h"
#include <string>
#include <optional>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class Class : public Compiler::CodeGen::Node
    {
    public:
        Class(std::shared_ptr<Java::Archive::ClassInfo> classInfo)
        {
            std::filesystem::path package = std::filesystem::path(classInfo->getName());
            if (package.has_parent_path())
            {
                name = package.filename();
                ns = Namespace(package.parent_path().string());
            }
            else
            {
                name = package.string();
                ns = std::nullopt;
            }
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
            if (ns.has_value())
            {
                return ns.value().asString() + Namespace::CPP_NAMESPACE_DELIMITER + name;
            }

            return name;
        }

        void addField(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Field> field)
        {
            childrens.emplace_back(field);
        }

        virtual void dump(std::ostream& outputStream) override
        {
            outputStream << fmt::format("struct {}", getName());
            outputStream << "\n{";
            for (const auto& child : childrens)
            {
                child->dump(outputStream);
            }
            outputStream << "\n};";
        }

    protected:
        std::optional<Namespace> ns;
        std::string name;
    };
}

#endif //SUPERJET_CLASS_H
