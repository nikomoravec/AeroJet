#ifndef SUPERJET_CONSTANTPOOLENTRY_H
#define SUPERJET_CONSTANTPOOLENTRY_H

#include <vector>

#include "Java/Types.h"

namespace SuperJet::Java::Archive
{
    enum class ConstantPoolInfoTag : JVM::u1
    {
        UTF_8 = 1,
        INTEGER = 3,
        FLOAT = 4,
        LONG = 5,
        DOUBLE = 6,
        CLASS = 7,
        STRING = 8,
        FIELD_REF = 9,
        METHOD_REF = 10,
        INTERFACE_METHOD_REF = 11,
        NAME_AND_TYPE = 12,
        METHOD_HANDLE = 15,
        METHOD_TYPE = 16,
        INVOKE_DYNAMIC = 18
    };

    class ConstantPoolEntry
    {
    public:
        ConstantPoolEntry(const ConstantPoolInfoTag inTag) : tag(inTag)
        {
        }

        ConstantPoolInfoTag getTag() const
        {
            return tag;
        }

    protected:
        ConstantPoolInfoTag tag;
    };

    class ConstantPoolInfoUtf8 : public ConstantPoolEntry
    {
    public:
        ConstantPoolInfoUtf8(const ConstantPoolInfoTag inTag, const std::vector<JVM::u1>& inBytes) : ConstantPoolEntry(inTag), bytes(inBytes)
        {
        }

        const std::vector<JVM::u1>& getBytes() const
        {
            return bytes;
        }

        JVM::u2 getLength()
        {
            return bytes.size();
        }

        std::string asString()
        {
            return {bytes.begin(), bytes.end()};
        }

    protected:
        std::vector<JVM::u1> bytes;
    };

    class ConstantPoolInfoClass : public ConstantPoolEntry
    {
    public:
        ConstantPoolInfoClass(const ConstantPoolInfoTag inTag, const JVM::u2 inNameIndex) : ConstantPoolEntry(inTag), nameIndex(inNameIndex)
        {
        }

        JVM::u2 getNameIndex() const
        {
            return nameIndex;
        }

    protected:
        JVM::u2 nameIndex;
    };

    class ConstantPoolInfoFieldRef : public ConstantPoolEntry
    {
    public:
        ConstantPoolInfoFieldRef(const ConstantPoolInfoTag inTag,const JVM::u2 inClassIndex, const JVM::u2 inNameAndTypeIndex) : ConstantPoolEntry(inTag), classIndex(inClassIndex), nameAndTypeIndex(inNameAndTypeIndex)
        {
        }

        JVM::u2 getClassIndex() const
        {
            return classIndex;
        }

        JVM::u2 getNameAndTypeIndex() const
        {
            return nameAndTypeIndex;
        }

    protected:
        JVM::u2 classIndex;
        JVM::u2 nameAndTypeIndex;
    };

    using ConstantPoolInfoMethodRef = ConstantPoolInfoFieldRef;

    using ConstantPoolInfoInterfaceMethodRef = ConstantPoolInfoFieldRef;

    class ConstantPoolInfoString : public ConstantPoolEntry
    {
    public:
        ConstantPoolInfoString(const ConstantPoolInfoTag inTag, const JVM::u2 inStringIndex) : ConstantPoolEntry(inTag), stringIndex(inStringIndex)
        {
        }

        JVM::u2 getStringIndex() const
        {
            return stringIndex;
        }

    protected:
        JVM::u2 stringIndex;
    };

    class ConstantPoolInfoInteger : public ConstantPoolEntry
    {
    public:
        ConstantPoolInfoInteger(const ConstantPoolInfoTag inTag, const JVM::u4 inBytes) : ConstantPoolEntry(inTag), bytes(inBytes)
        {
        }

        JVM::u4 getBytes() const
        {
            return bytes;
        }

    protected:
        JVM::u4 bytes;
    };

    using ConstantPoolFloat = ConstantPoolInfoInteger;

    class ConstantPoolInfoLong : public ConstantPoolEntry
    {
    public:
        ConstantPoolInfoLong(const ConstantPoolInfoTag inTag, const JVM::u4 inHighBytes, const JVM::u4 inLowBytes) : ConstantPoolEntry(inTag), highBytes(inHighBytes), lowBytes(inLowBytes)
        {
        }

        JVM::u4 getHighBytes() const
        {
            return highBytes;
        }

        JVM::u4 getLowBytes() const
        {
            return lowBytes;
        }

        int64_t asLong() const
        {
            union Long
            {
            public:
                Long(JVM::u4 highBytes, JVM::u4 lowBytes)
                {
                    parts[0] = highBytes;
                    parts[1] = lowBytes;
                }

                int64_t getValue() const
                {
                    return value;
                }

            protected:
                int64_t value;
                JVM::u4 parts[2] {};
            };

            return Long(highBytes, lowBytes).getValue();
        }

        double asDouble() const
        {
            return static_cast<double>(asLong());
        }

    protected:
        JVM::u4 highBytes;
        JVM::u4 lowBytes;
    };

    using ConstantPoolInfoDouble = ConstantPoolInfoLong;

    class ConstantPoolInfoNameAndType : public ConstantPoolEntry
    {
    public:
        ConstantPoolInfoNameAndType(const ConstantPoolInfoTag inTag, const JVM::u2 inNameIndex, const JVM::u2 inDescriptorIndex) : ConstantPoolEntry(inTag), nameIndex(inNameIndex), descriptorIndex(inDescriptorIndex)
        {
        }

        JVM::u2 getNameIndex() const
        {
            return nameIndex;
        }

        JVM::u2 getDescriptorIndex() const
        {
            return descriptorIndex;
        }

    protected:
        JVM::u2 nameIndex;
        JVM::u2 descriptorIndex;
    };

    class ConstantPoolInfoMethodHandle : public ConstantPoolEntry
    {
    public:
        enum class ReferenceKind : JVM::u1
        {
            REF_getField = 1,
            REF_getStatic = 2,
            REF_putField = 3,
            REF_putStatic = 4,
            REF_invokeVirtual = 5,
            REF_invokeStatic = 6,
            REF_invokeSpecial = 7,
            REF_newInvokeSpecial = 8,
            REF_invokeInterface = 9
        };

        ConstantPoolInfoMethodHandle(const ConstantPoolInfoTag inTag, const JVM::u1 inReferenceKind, const JVM::u2 inReferenceIndex) : ConstantPoolEntry(inTag), referenceKind(inReferenceKind), referenceIndex(inReferenceIndex)
        {
        }

        ReferenceKind getReferenceKind() const
        {
            return static_cast<ReferenceKind>(referenceKind);
        }

        JVM::u2 getReferenceIndex() const
        {
            return referenceIndex;
        }

    protected:
        JVM::u1 referenceKind;
        JVM::u2 referenceIndex;
    };

    class ConstantPoolInfoMethodType : public ConstantPoolEntry
    {
    public:
        ConstantPoolInfoMethodType(const ConstantPoolInfoTag inTag, const JVM::u2 inDescriptorIndex) : ConstantPoolEntry(inTag), descriptorIndex(inDescriptorIndex)
        {
        }

        JVM::u2 getDescriptorIndex() const
        {
            return descriptorIndex;
        }

    protected:
        JVM::u2 descriptorIndex;
    };

    class ConstantPoolInfoInvokeDynamic : public ConstantPoolEntry
    {
    public:
        ConstantPoolInfoInvokeDynamic(const ConstantPoolInfoTag inTag, const JVM::u2, const JVM::u2) : ConstantPoolEntry(inTag)
        {
        }

        JVM::u2 getBootstrapMethodAttributeIndex() const
        {
            return bootstrapMethodAttributeIndex;
        }

        JVM::u2 getNameAndTypeIndex() const
        {
            return nameAndTypeIndex;
        }

    protected:
        JVM::u2 bootstrapMethodAttributeIndex;
        JVM::u2 nameAndTypeIndex;
    };
}

#endif //SUPERJET_CONSTANTPOOLENTRY_H
