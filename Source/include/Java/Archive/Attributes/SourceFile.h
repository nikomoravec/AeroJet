#ifndef SUPERJET_SOURCEFILE_H
#define SUPERJET_SOURCEFILE_H

#include "Java/Types.h"
#include "Java/Archive/ConstantPool.h"
#include "Java/Archive/AttributeInfo.h"

namespace SuperJet::Java::Archive::Attributes
{
    class SourceFile
    {
    public:
        static constexpr auto SOURCE_FILE_ATTRIBUTE_NAME = "SourceFile";

    public:
        SourceFile(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        JVM::u2 getAttributeNameIndex() const
        {
            return attributeNameIndex;
        }

        JVM::u4 getAttributeLength() const
        {
            return attributeLength;
        }

        JVM::u2 getSourceFileIndex() const
        {
            return sourceFileIndex;
        }

        const std::string& getSourceFileName() const
        {
            return sourceFileName;
        }

    protected:
        JVM::u2 attributeNameIndex;
        JVM::u4 attributeLength;
        JVM::u2 sourceFileIndex;
        std::string sourceFileName;
    };
}

#endif //SUPERJET_SOURCEFILE_H
