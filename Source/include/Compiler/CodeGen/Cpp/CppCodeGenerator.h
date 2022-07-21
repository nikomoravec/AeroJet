#ifndef SUPERJET_CPPCODEGENERATOR_H
#define SUPERJET_CPPCODEGENERATOR_H

#include "Compiler/CodeGen/CodeGenerator.h"
#include "Compiler/CodeGen/Node.h"
#include "Compiler/CodeGen/Cpp/Namespace.h"
#include "fmt/format.h"

#include <filesystem>
#include <queue>
#include <memory>

namespace SuperJet::Compiler::CodeGen::Cpp
{
    class CppCodeGenerator : public CodeGenerator<>
    {
    public:
        CppCodeGenerator(const std::filesystem::path& output) : tabMultiplier(0), outputFile(output)
        {
        }

        std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Namespace> addNamespace(const std::filesystem::path& package)
        {
            std::shared_ptr<SuperJet::Compiler::CodeGen::Cpp::Namespace> node = std::make_shared<SuperJet::Compiler::CodeGen::Cpp::Namespace>(package);
            nodes.push(node);
            return node;
        }

    protected:
        int tabMultiplier;
        std::filesystem::path outputFile;
        std::ofstream outputStream;
        std::queue<std::shared_ptr<SuperJet::Compiler::CodeGen::Node>> nodes;
    };
}

#endif //SUPERJET_CPPCODEGENERATOR_H
