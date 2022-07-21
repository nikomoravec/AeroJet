#ifndef SUPERJET_METHODINFO_H
#define SUPERJET_METHODINFO_H

#include "Java/Archive/AttributeInfo.h"
#include "Java/Types.h"

namespace SuperJet::Java::Archive
{
    class MethodInfo
    {
    public:
        enum class AccessFlags : JVM::u2
        {
            ACC_PUBLIC = 0x0001,
            ACC_PRIVATE = 0x0002,
            ACC_PROTECTED = 0x0004,
            ACC_STATIC = 0x0008,
            ACC_FINAL = 0x0010,
            ACC_SYNCHRONIZED = 0x0020,
            ACC_BRIDGE = 0x0040,
            ACC_VARARGS = 0x0080,
            ACC_NATIVE = 0x0100,
            ACC_ABSTRACT = 0x0400,
            ACC_STRICT = 0x0800,
            ACC_SYNTHETIC = 0x1000
        };

        MethodInfo(const JVM::u2 inAccessFlags, const JVM::u2 inNameIndex, const JVM::u2 inDescriptorIndex, const std::vector<AttributeInfo>& inAttributes) : accessFlags(inAccessFlags), nameIndex(inNameIndex), descriptorIndex(inDescriptorIndex), attributes(inAttributes)
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

    protected:
        JVM::u2 accessFlags;
        JVM::u2 nameIndex;
        JVM::u2 descriptorIndex;
        std::vector<AttributeInfo> attributes;
    };
}

#endif //SUPERJET_METHODINFO_H
