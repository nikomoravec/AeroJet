#ifndef SUPERJET_RUNTIMEEXCEPTION_H
#define SUPERJET_RUNTIMEEXCEPTION_H

#include <stdexcept>
#include <string>

#include "backward.hpp"

namespace SuperJet::Compiler
{
    class RuntimeException : public  std::runtime_error
    {
    public:
        RuntimeException(const std::string& description) : std::runtime_error(description)
        {
            ::backward::StackTrace stackTrace;
            stackTrace.load_here(128);
            ::backward::Printer stackTracePrinter;
            stackTracePrinter.print(stackTrace);
        }
    };
}

#endif //SUPERJET_RUNTIMEEXCEPTION_H
