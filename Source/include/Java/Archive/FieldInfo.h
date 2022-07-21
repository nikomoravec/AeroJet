#ifndef SUPERJET_FIELDINFO_H
#define SUPERJET_FIELDINFO_H

#include <vector>

#include "AttributeInfo.h"
#include "Java/Types.h"

namespace SuperJet::Java::Archive
{
    class FieldInfo
    {
    public:
        enum class AccessFlags : JVM::u2
        {
            ACC_PUBLIC = 0x0001,
            ACC_PRIVATE = 0x0002,
            ACC_PROTECTED = 0x0004,
            ACC_STATIC = 0x0008,
            ACC_FINAL = 0x0010,
            ACC_VOLATILE = 0x0040,
            ACC_TRANSIENT = 0x0080,
            ACC_SYNTHETIC = 0x1000,
            ACC_ENUM = 0x4000
        };

        FieldInfo(const JVM::u2 inAccessFlags, const JVM::u2 inNameIndex, const JVM::u2 inDescriptorIndex, const std::vector<AttributeInfo>& inAttributes) : accessFlags(inAccessFlags), nameIndex(inNameIndex), descriptorIndex(inDescriptorIndex), attributes(inAttributes)
        {
        }

        AccessFlags getAccessFlags() const
        {
            return static_cast<AccessFlags>(accessFlags);
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

        bool isStatic() const
        {
            return accessFlags & static_cast<JVM::u2>(AccessFlags::ACC_STATIC);
        }

    protected:
        JVM::u2 accessFlags;
        JVM::u2 nameIndex;
        JVM::u2 descriptorIndex;
        std::vector<AttributeInfo> attributes;
    };
}

#endif //SUPERJET_FIELDINFO_H
