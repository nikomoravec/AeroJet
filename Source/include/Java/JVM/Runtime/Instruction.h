#ifndef SUPERJET_OPCODE_H
#define SUPERJET_OPCODE_H

#include "Java/Types.h"
#include "Java/JVM/Runtime/Operations.h"
#include "Compiler/Exceptions/RuntimeException.h"

#include <vector>

namespace SuperJet::Java::JVM::Runtime
{
    class Instruction
    {
    public:
        Instruction(OperationCode inOpCode) : opCode(inOpCode)
        {
        }

        Instruction(OperationCode inOpCode, std::vector<JVM::u1>&& inData) : opCode(inOpCode), data(std::move(inData))
        {
        }

    protected:
        OperationCode opCode;
        std::vector<JVM::u1> data;
    };
}

#endif // SUPERJET_OPCODE_H
