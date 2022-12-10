#include "Java/Archive/Attributes/Code.h"
#include "Java/Archive/Archive.h"
#include "Compiler/Exceptions/IncorrectAttributeTypeException.h"
#include "fmt/format.h"

namespace SuperJet::Java::Archive::Attributes
{
    Code::Code(const ConstantPool& constantPool, const AttributeInfo& attributeInfo)
    {
        const JVM::u2& nameIndex = attributeInfo.getAttributeNameIndex();
        const std::string attributeName = constantPool.get<ConstantPoolInfoUtf8>(nameIndex)->asString();

        if (attributeName == CODE_ATTRIBUTE_NAME)
        {
            attributeNameIndex = nameIndex;
            attributeLength = attributeInfo.getSize();

            const std::vector<JVM::u1>& info = attributeInfo.getInfo();

            std::stringstream ss;
            std::move(info.begin(), info.end(),std::ostream_iterator<JVM::u1>(ss));

            maxStack = read<JVM::u2>(ss);
            maxLocals = read<JVM::u2>(ss);

            const JVM::u4 codeLength = read<JVM::u4>(ss);

            const JVM::u4 currentPos = static_cast<JVM::u4>(ss.tellg());
            const JVM::u4 endPos = currentPos + codeLength;
            const JVM::u4 firstOpCodeOffset = currentPos + attributeInfo.getInfoOffset();
            const JVM::u4 firstOpCodeLocalOffset = currentPos + 1;
            while (ss.tellg() != endPos)
            {
                code.emplace_back(read<std::shared_ptr<SuperJet::Java::JVM::Runtime::Operation>>(ss, firstOpCodeOffset, firstOpCodeLocalOffset));
            }

            const JVM::u2& exceptionTableLength = read<JVM::u2>(ss);
            exceptionTable.reserve(exceptionTableLength);
            for (int32_t exceptionTableIndex = 0; exceptionTableIndex < exceptionTableLength; exceptionTableIndex++)
            {
                exceptionTable.emplace_back(ExceptionTableEntry{read<JVM::u2>(ss), read<JVM::u2>(ss), read<JVM::u2>(ss), read<JVM::u2>(ss)});
            }

            const JVM::u2& attributesCount = read<JVM::u2>(ss);
            for (int32_t attributeIndex = 0; attributeIndex < attributesCount; attributeIndex++)
            {
                attributes.emplace_back(read<AttributeInfo>(ss));
            }
        }
        else
        {
            throw SuperJet::Compiler::IncorrectAttributeTypeException(CODE_ATTRIBUTE_NAME, attributeName);
        }
    }
}