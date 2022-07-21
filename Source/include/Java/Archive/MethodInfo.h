#ifndef SUPERJET_METHODINFO_H
#define SUPERJET_METHODINFO_H

#include "Java/Archive/AttributeInfo.h"
#include "Java/Types.h"

namespace SuperJet::Java::Archive
{
    class MethodInfo
    {
    public:
        MethodInfo(const JVM::u2 inAccessFlags, const JVM::u2 inNameIndex, const JVM::u2 inDescriptorIndex, const std::vector<AttributeInfo>& inAttributes) : accessFlags(inAccessFlags), nameIndex(inNameIndex), descriptorIndex(inDescriptorIndex), attributes(inAttributes)
        {
        }

        JVM::u2 getAccessFlags() const
        {
            return accessFlags;
        }

        JVM::u2 getNameIndex() const
        {
            return nameIndex;
        }

        JVM::u2 getDescriptorIndex() const
        {
            return descriptorIndex;
        }

        const std::vector<AttributeInfo>& getAttributes() const
        {
            return attributes;
        }

    protected:
        JVM::u2 accessFlags;
        JVM::u2 nameIndex;
        JVM::u2 descriptorIndex;
        std::vector<AttributeInfo> attributes;
    };
}

#endif //SUPERJET_METHODINFO_H
