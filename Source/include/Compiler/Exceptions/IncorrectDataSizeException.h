#ifndef SUPERJET_INCORRECTDATASIZEEXCEPTION_H
#define SUPERJET_INCORRECTDATASIZEEXCEPTION_H

#include "RuntimeException.h"
#include "fmt/format.h"

namespace SuperJet::Compiler
{
    class IncorrectDataSizeException : public RuntimeException
    {
    public:
        IncorrectDataSizeException(uint32_t expected, uint32_t given) : RuntimeException(fmt::format("Invalid data size! Expected size: {} but got {}", expected, given))
        {
        }
    };
}

#endif //SUPERJET_INCORRECTDATASIZEEXCEPTION_H
