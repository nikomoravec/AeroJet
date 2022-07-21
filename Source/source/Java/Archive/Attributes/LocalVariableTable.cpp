#include "Java/Archive/Attributes/LocalVariableTable.h"
#include "Java/Archive/Archive.h"
#include "Compiler/Exceptions/IncorrectAttributeTypeException.h"

namespace SuperJet::Java::Archive::Attributes
{
    LocalVariableTable::LocalVariableTable(const SuperJet::Java::Archive::ConstantPool& constantPool, const SuperJet::Java::Archive::AttributeInfo& attributeInfo)
    {
        const JVM::u2& nameIndex = attributeInfo.getAttributeNameIndex();
        const std::string attributeName = constantPool.get<ConstantPoolInfoUtf8>(nameIndex)->asString();

        if (attributeName == LOCAL_VARIABLE_TABLE_ATTRIBUTE_NAME)
        {
            attributeNameIndex = nameIndex;
            attributeLength = attributeInfo.getSize();

            const std::vector<JVM::u1>& info = attributeInfo.getInfo();

            std::stringstream ss;
            std::move(info.begin(), info.end(),std::ostream_iterator<JVM::u1>(ss));

            JVM::u2 localVariableTableLength = read<JVM::u2>(ss);
            localVariableTable.reserve(localVariableTableLength);

            for (int32_t localVariableTableIndex = 0; localVariableTableIndex < localVariableTableLength; localVariableTableIndex++)
            {
                JVM::u2 entryStartPc = read<JVM::u2>(ss);
                JVM::u2 entryLength = read<JVM::u2>(ss);
                JVM::u2 entryNameIndex = read<JVM::u2>(ss);
                JVM::u2 entryDescriptorIndex = read<JVM::u2>(ss);
                JVM::u2 entryIndex = read<JVM::u2>(ss);

                localVariableTable.emplace_back(LocalVariableTableEntry{entryStartPc, entryLength, entryNameIndex, entryDescriptorIndex, entryIndex});
            }
        }
        else
        {
            throw SuperJet::Compiler::IncorrectAttributeTypeException(LOCAL_VARIABLE_TABLE_ATTRIBUTE_NAME, attributeName);
        }
    }
}

