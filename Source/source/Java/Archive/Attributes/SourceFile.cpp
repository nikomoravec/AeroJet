#include "Java/Archive/Attributes/SourceFile.h"
#include "Java/Archive/Archive.h"
#include "Compiler/Exceptions/IncorrectAttributeTypeException.h"
#include "fmt/format.h"

namespace SuperJet::Java::Archive::Attributes
{
    SourceFile::SourceFile(const ConstantPool& constantPool, const AttributeInfo& attributeInfo)
    {
        const JVM::u2& nameIndex = attributeInfo.getAttributeNameIndex();
        const std::string& attributeName = constantPool.get<ConstantPoolInfoUtf8>(nameIndex)->asString();

        if (attributeName == SOURCE_FILE_ATTRIBUTE_NAME)
        {
            attributeNameIndex = nameIndex;
            attributeLength = attributeInfo.getSize();

            const std::vector<JVM::u1>& info = attributeInfo.getInfo();

            std::stringstream ss;
            std::move(info.begin(), info.end(),std::ostream_iterator<JVM::u1>(ss));

            sourceFileIndex = read<JVM::u2>(ss);
            sourceFileName = constantPool.get<ConstantPoolInfoUtf8>(sourceFileIndex)->asString();
        }
        else
        {
            throw SuperJet::Compiler::IncorrectAttributeTypeException(SOURCE_FILE_ATTRIBUTE_NAME, attributeName);
        }
    }
}