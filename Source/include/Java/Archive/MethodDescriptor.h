#ifndef SUPERJET_JVM_H
#define SUPERJET_JVM_H

#include <string>
#include <vector>
#include "Java/Types.h"
#include "Java/Archive/FieldDescriptor.h"

namespace SuperJet::Java::Archive
{
    class MethodDescriptor
    {
    public:
        enum class Type : JVM::u1
        {
            VOID = 'V',
            FIELD
        };

        static constexpr char METHOD_DESCRIPTOR_ARGS_BEGIN_TOKEN = '(';
        static constexpr char METHOD_DESCRIPTOR_ARGS_END_TOKEN = ')';

    public:
        MethodDescriptor(const std::string& inDescriptor);

        std::optional<FieldDescriptor> getReturnType() const
        {
            return returnType;
        }

        std::vector<FieldDescriptor> getArguments() const
        {
            return arguments;
        }

        std::string getRawLiteral() const
        {
            return rawLiteral;
        }

    protected:
        std::optional<FieldDescriptor> returnType;
        std::vector<FieldDescriptor> arguments;
        std::string rawLiteral;
    };
}

#endif //SUPERJET_JVM_H
