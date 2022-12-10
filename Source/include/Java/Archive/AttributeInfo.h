#ifndef SUPERJET_ATTRIBUTEINFO_H
#define SUPERJET_ATTRIBUTEINFO_H

#include <vector>

#include "Java/Types.h"

namespace SuperJet::Java::Archive
{
    class AttributeInfo
    {
    public:
        AttributeInfo(const JVM::u2 inAttributeIndex, const std::vector<JVM::u1>& inInfo, const JVM::u4 inInfoOffset) : attributeNameIndex(inAttributeIndex), info(inInfo), infoOffset(inInfoOffset)
        {
        }

        JVM::u2 getAttributeNameIndex() const { return attributeNameIndex; }
        JVM::u2 getSize() const { return info.size(); }
        JVM::u4 getInfoOffset() const { return infoOffset; }
        const std::vector<JVM::u1>& getInfo() const { return info; }

    protected:
        JVM::u2 attributeNameIndex;
        std::vector<JVM::u1> info;
        JVM::u4 infoOffset;
    };
}

#endif //SUPERJET_ATTRIBUTEINFO_H
