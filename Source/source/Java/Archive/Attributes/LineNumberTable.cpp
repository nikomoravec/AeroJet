#include "Java/Archive/Attributes/LineNumberTable.h"

namespace SuperJet::Java::Archive::Attributes
{
    LineNumberTable::LineNumberTable(const ConstantPool &constantPool, const AttributeInfo &attributeInfo)
    {
        const JVM::u2& nameIndex = attributeInfo.getAttributeNameIndex();
        std::string attributeName = constantPool.get<ConstantPoolInfoUtf8>(nameIndex)->asString();

        if (attributeName == LINE_NUMBER_TABLE_ATTRIBUTE_NAME)
        {
            attributeNameIndex = nameIndex;
            attributeLength = attributeInfo.getSize();

            const std::vector<JVM::u1>& info = attributeInfo.getInfo();

            std::stringstream ss;
            std::move(info.begin(), info.end(), std::ostream_iterator<JVM::u1>(ss));

            const JVM::u2 lineNumberTableLength = read<JVM::u2>(ss);
            lineNumberTable.reserve(lineNumberTableLength);
            for (int32_t LineNumberTableEntryIndex = 0; LineNumberTableEntryIndex < lineNumberTableLength; LineNumberTableEntryIndex++)
            {
                lineNumberTable.emplace_back(LineNUmberTableEntry{read<JVM::u2>(ss), read<JVM::u2>(ss)});
            }
        }
        else
        {
            throw SuperJet::Compiler::IncorrectAttributeTypeException(LINE_NUMBER_TABLE_ATTRIBUTE_NAME, attributeName);
        }
    }
}