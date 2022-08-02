#ifndef SUPERJET_CODEGEN_NAMESPACE_H
#define SUPERJET_CODEGEN_NAMESPACE_H

#include "Compiler/Compiler.h"
#include "Compiler/Exceptions/RuntimeException.h"
#include "Compiler/CodeGen/Node.h"
#include "Utils/StringUtils.h"

#include <string>
#include <vector>
#include <strstream>
#include <filesystem>
#include <stack>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class ForwardDeclaration;
    class Class;

    class Namespace : public Compiler::CodeGen::Node
    {
    public:
        static constexpr char JAVA_PACKAGE_DELIMITER = '/';
        static constexpr auto CPP_NAMESPACE_DELIMITER = "::";

        Namespace (const std::filesystem::path& package, bool packageContainsClassName = true)
        {
            std::vector<std::string> split = SuperJet::Utils::StringUtils::split(package.string(), JAVA_PACKAGE_DELIMITER);
            if (split.size() < 1)
            {
                throw SuperJet::Compiler::RuntimeException("Namespace can not be empty!");
            }

            if (packageContainsClassName == true)
            {
                parts = std::vector<std::string>(split.begin(), split.end() - 1);
            }
            else
            {
                parts = std::vector<std::string>(split.begin(), split.end());
            }
            
            return;
        }

        std::vector<std::string> getParts() const
        {
            return parts;
        }

        void addForwardDeclaration(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::ForwardDeclaration> forwardDeclaration);

        void addClass(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Class> clazz);

        std::string asString() const
        {
            std::stringstream ss;

            ss << parts[0];
            for (int32_t index = 1; index < parts.size(); index++)
            {
                ss << CPP_NAMESPACE_DELIMITER << parts[index];
            }

            return ss.str();

        }

        virtual void dump(std::ostream& outputStream) override
        {           
            outputStream << fmt::format("namespace {}\n", asString());
            outputStream << "{\n";

            for (const auto& child : childrens)
            {
                child->dump(outputStream);
            }

            outputStream << "\n}\n";
        }

    protected:
        std::vector<std::string> parts;
    };
}

#endif //SUPERJET_CODEGEN_NAMESPACE_H
