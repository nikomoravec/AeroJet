#ifndef SUPERJET_CLASS_H
#define SUPERJET_CLASS_H

#include <variant>
#include <vector>

#include "Java/Archive/ConstantPool.h"
#include "Java/Types.h"
#include "FieldInfo.h"
#include "MethodInfo.h"

namespace SuperJet::Java::Archive
{
    static constexpr JVM::u4 JAVA_CLASS_MAGIC  = 0xCAFEBABE;
    static constexpr JVM::u2 MAX_JAVA_CLASS_MAJOR_VERSION = 52; // 52 Java 8

    class Class
    {
    public:
        enum class AccessFlags : JVM::u2
        {
            ACC_PUBLIC     = 0x0001,
            ACC_FINAL      = 0x0010,
            ACC_SUPER      = 0x0020,
            ACC_INTERFACE  = 0x0200,
            ACC_ABSTRACT   = 0x0400,
            ACC_SYNTHETIC  = 0x1000,
            ACC_ANNOTATION = 0x2000,
            ACC_ENUM       = 0x4000
        };

        Class(const JVM::u4 inMagic, const JVM::u2 inMinorVersion, const JVM::u2 inMajorVersion,
              const ConstantPool& inConstantPool, const JVM::u2 inAccessFlags, const JVM::u2 inThisClass,
              const JVM::u2 inSuperClass, const std::vector<JVM::u2>& inInterfaces, const std::vector<FieldInfo>& inFields,
              const std::vector<MethodInfo>& inMethods, const std::vector<AttributeInfo>& inAttributes)
              : magic(inMagic), minorVersion(inMinorVersion), majorVersion(inMajorVersion), constantPool(inConstantPool),
              accessFlags(inAccessFlags), thisClass(inThisClass), superClass(inSuperClass),
              interfaces(inInterfaces), fields(inFields), methods(inMethods), attributes(inAttributes)
        {
        }

        JVM::u4 getMagic() const
        {
            return magic;
        }

        JVM::u2 getMinorVersion() const
        {
            return minorVersion;
        }

        JVM::u2 getMajorVersion() const
        {
            return majorVersion;
        }

        const ConstantPool& getConstantPool() const
        {
            return constantPool;
        }

        JVM::u2 getAccessFlags() const
        {
            return accessFlags;
        }

        JVM::u2 getThisClass() const
        {
            return thisClass;
        }

        JVM::u2 getSuperClass() const
        {
            return superClass;
        }

        const std::vector<JVM::u2>& getInterfaces() const
        {
            return interfaces;
        }

        const std::vector<FieldInfo>& getFields() const
        {
            return fields;
        }

        const std::vector<MethodInfo>& getMethods() const
        {
            return methods;
        }

        const std::vector<AttributeInfo>& getAttributes() const
        {
            return attributes;
        }

    protected:
        JVM::u4 magic;
        JVM::u2 minorVersion;
        JVM::u2 majorVersion;
        ConstantPool constantPool;
        JVM::u2 accessFlags;
        JVM::u2 thisClass;
        JVM::u2 superClass;
        std::vector<JVM::u2> interfaces;
        std::vector<FieldInfo> fields;
        std::vector<MethodInfo> methods;
        std::vector<AttributeInfo> attributes;
    };
}

#endif //SUPERJET_CLASS_H
