#include "Java/Archive/Attributes/ConstantValue.h"

namespace SuperJet::Java::Archive::Attributes
{
    ConstantValue::ConstantValue(const ConstantPool &constantPool, const AttributeInfo &attributeInfo)
    {
        const JVM::u2& nameIndex = attributeInfo.getAttributeNameIndex();
        std::string attributeName = constantPool.get<ConstantPoolInfoUtf8>(nameIndex)->asString();

        if (attributeName == CONSTANT_VALUE_ATTRIBUTE_NAME)
        {
            attributeNameIndex = nameIndex;
            attributeLength = attributeInfo.getSize();

            const std::vector<JVM::u1>& info = attributeInfo.getInfo();

            std::stringstream ss;
            std::move(info.begin(), info.end(), std::ostream_iterator<JVM::u1>(ss));

            constantValueIndex = read<JVM::u2>(ss);
        }
        else
        {
            throw SuperJet::Compiler::IncorrectAttributeTypeException(CONSTANT_VALUE_ATTRIBUTE_NAME, attributeName);
        }
    }
}
