#ifndef SUPERJET_NAMESPACE_H
#define SUPERJET_NAMESPACE_H

#include "Compiler/Compiler.h"
#include "Compiler/Exceptions/RuntimeException.h"
#include "Compiler/CodeGen/Node.h"

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
        static constexpr auto JAVA_PACKAGE_DELIMITER = '/';
        static constexpr auto CPP_NAMESPACE_DELIMITER = "::";

        Namespace(const std::filesystem::path& package)
        {   
            for (auto it = package.begin(); it != package.end(); ++it)
            {
                parts.push((*it).string());
            }

            if (parts.empty())
            {
                throw SuperJet::Compiler::RuntimeException("Namespace can not be empty!");
            }
        }

        std::queue<std::string> getParts() const
        {
            return parts;
        }

        void addForwardDeclaration(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::ForwardDeclaration> forwardDeclaration);

        void addClass(std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Class> clazz);

        std::string asString() const
        {
            std::queue<std::string> p = getParts();

            std::stringstream ss;

            ss << p.front();
            p.pop();

            if (p.empty())
            {
                return ss.str();
            }

            while(!p.empty())
            {
                ss << CPP_NAMESPACE_DELIMITER << p.front();
                p.pop();
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
        std::queue<std::string> parts;
    };
}

#endif //SUPERJET_NAMESPACE_H
