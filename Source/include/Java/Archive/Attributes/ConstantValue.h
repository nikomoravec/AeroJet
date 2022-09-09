#ifndef SUPERJET_CONSTANTVALUE_H
#define SUPERJET_CONSTANTVALUE_H

#include "Compiler/Exceptions/IncorrectAttributeTypeException.h"
#include "Java/Archive/Archive.h"
#include "Java/Archive/AttributeInfo.h"
#include "Java/Archive/ConstantPool.h"
#include "Java/Types.h"

#include <vector>

namespace SuperJet::Java::Archive::Attributes
{
    class ConstantValue
    {
    public:
        static constexpr auto CONSTANT_VALUE_ATTRIBUTE_NAME = "ConstantValue";

        ConstantValue(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        JVM::u2 getAttributeNameIndex() const
        {
            return attributeNameIndex;
        }

        JVM::u4 getAttributeLength() const
        {
            return attributeLength;
        }

        JVM::u2 getConstantValueIndex() const
        {
            return constantValueIndex;
        }

    protected:
        JVM::u2 attributeNameIndex;
        JVM::u4 attributeLength;
        JVM::u2 constantValueIndex;
    };
}

#endif // SUPERJET_CONSTANTVALUE_H