#include "Compiler/CodeGen/Cpp/Namespace.h"

#include <string>
#include <stack>

namespace SuperJet
{
    template<>
    std::string toString(const SuperJet::Compiler::CodeGen::Cpp::Namespace& object)
    {
        std::stack<std::string> parts = object.getParts();
        std::stringstream ss;

        ss << parts.top();
        parts.pop();

        if (parts.empty())
        {
            return ss.str();
        }

        while(!parts.empty())
        {
            ss << "::" << parts.top();
            parts.pop();
        }

        return ss.str();
    }
}