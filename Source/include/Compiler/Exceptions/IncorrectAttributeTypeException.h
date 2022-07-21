#ifndef SUPERJET_INCORRECTATTRIBUTETYPEEXCEPTION_H
#define SUPERJET_INCORRECTATTRIBUTETYPEEXCEPTION_H

#include "RuntimeException.h"
#include "fmt/format.h"
#include <string>

namespace SuperJet::Compiler
{
    class IncorrectAttributeTypeException : public RuntimeException
    {
    public:
        IncorrectAttributeTypeException(const std::string& expectedType, const std::string& givenType) : RuntimeException(fmt::format("Incorrect attribute type! Expected: \'{}\' but got \'{}\'", expectedType, givenType))
        {
        }
    };
}

#endif //SUPERJET_INCORRECTATTRIBUTETYPEEXCEPTION_H
