#ifndef SUPERJET_OPERATIONNOTSUPPORTEDEXCEPTION_H
#define SUPERJET_OPERATIONNOTSUPPORTEDEXCEPTION_H

#include "NotSupportedException.h"
#include "Java/JVM/Runtime/Operations.h"

namespace SuperJet::Compiler
{
    class OperationNotSupportedException : public NotSupportedException
    {
    public:
        OperationNotSupportedException(Java::JVM::Runtime::OperationCode opCode) : NotSupportedException(fmt::format("Operation {:#06x} not supported!", static_cast<SuperJet::Java::JVM::u1>(opCode)))
        {
        }
    };
}

#endif //SUPERJET_OPERATIONNOTSUPPORTEDEXCEPTION_H
