#ifndef SUPERJET_GODEGEN_NODE_H
#define SUPERJET_GODEGEN_NODE_H

#include "Compiler/Printer.h"

#include <memory>
#include <vector>

namespace SuperJet::Compiler::CodeGen
{
    class Node
    {
    public:
        virtual void dump(std::ostream& outputStream)
        {
            for (const std::shared_ptr<SuperJet::Compiler::CodeGen::Node> childNode : childrens)
            {
                childNode->dump(outputStream);
            }
        }

        void addNode(std::shared_ptr<SuperJet::Compiler::CodeGen::Node> node)
        {
            childrens.emplace_back(node);
        }

    protected:
        Node() = default;

    protected:
        std::vector<std::shared_ptr<SuperJet::Compiler::CodeGen::Node>> childrens;
    };
}

#endif