#ifndef SUPERJET_NAMESPACE_H
#define SUPERJET_NAMESPACE_H

#include "Compiler/Compiler.h"
#include "Compiler/Exceptions/RuntimeException.h"

#include <string>
#include <vector>
#include <strstream>
#include <filesystem>
#include <stack>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class Namespace
    {
    public:
        static constexpr auto JAVA_PACKAGE_DELIMITER = '/';
        static constexpr auto CPP_NAMESPACE_DELIMITER = "::";

        Namespace(const std::filesystem::path& package)
        {
            if (package.empty())
            {
                throw SuperJet::Compiler::RuntimeException("Namespace can not be empty!");
            }

            std::filesystem::path p = package;
            while (p.has_parent_path())
            {
                parts.push(p.filename());
                p = p.parent_path();
            }

            parts.push(p.filename());
        }

        std::stack<std::string> getParts() const
        {
            return parts;
        }

        void add(const std::string& part)
        {
            parts.push(part);
        }

    protected:
        std::stack<std::string> parts;
    };
}

#endif //SUPERJET_NAMESPACE_H
