#ifndef SUPERJET_NOTSUPPORTEDEXCEPTION_H
#define SUPERJET_NOTSUPPORTEDEXCEPTION_H

#include "RuntimeException.h"
#include <string>

namespace SuperJet::Compiler
{
    class NotSupportedException : public RuntimeException
    {
    public:
        NotSupportedException(const std::string& message) : RuntimeException(message)
        {
        }
    };
}

#endif //SUPERJET_NOTSUPPORTEDEXCEPTION_H
