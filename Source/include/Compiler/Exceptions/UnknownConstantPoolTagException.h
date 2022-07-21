#ifndef SUPERJET_UNKNOWNCONSTANTPOOLTAGEXCEPTION_H
#define SUPERJET_UNKNOWNCONSTANTPOOLTAGEXCEPTION_H

#include "Java/Types.h"
#include "RuntimeException.h"
#include "fmt/format.h"
#include <string>

namespace SuperJet::Compiler
{
    class UnknownConstantPoolTagException : public RuntimeException
    {
    public:
        UnknownConstantPoolTagException(SuperJet::Java::Archive::ConstantPoolInfoTag type) : RuntimeException(fmt::format("Unknown constant pool type {:#04x}", static_cast<Java::JVM::u1>(type)))
        {
        }
    };
}

#endif //SUPERJET_UNKNOWNCONSTANTPOOLTAGEXCEPTION_H
