#ifndef SUPERJET_CLASSINFO_H
#define SUPERJET_CLASSINFO_H

#include <variant>
#include <vector>
#include <optional>

#include "Java/Archive/ConstantPool.h"
#include "Java/Types.h"
#include "FieldInfo.h"
#include "MethodInfo.h"
#include "fmt/format.h"

#include "Utils/StringUtils.h"

namespace SuperJet::Java::Archive
{
    static constexpr JVM::u4 JAVA_CLASS_MAGIC  = 0xCAFEBABE;
    static constexpr JVM::u2 MAX_JAVA_CLASS_MAJOR_VERSION = 52; // 52 Java 8

    class ClassInfo
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

        ClassInfo(const JVM::u4 inMagic, const JVM::u2 inMinorVersion, const JVM::u2 inMajorVersion,
                  const ConstantPool& inConstantPool, const JVM::u2 inAccessFlags, const JVM::u2 inThisClass,
                  const std::optional<JVM::u2> inSuperClass, const std::vector<JVM::u2>& inInterfaces, const std::vector<FieldInfo>& inFields,
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

        AccessFlags getAccessFlags() const
        {
            return static_cast<AccessFlags>(accessFlags);
        }

        JVM::u2 getThisClass() const
        {
            return thisClass;
        }

        bool isSuperClassPresented() const
        {
            return superClass.has_value();
        }

        JVM::u2 getSuperClass() const
        {
            if (!isSuperClassPresented())
            {
                std::shared_ptr<ConstantPoolInfoClass> thisClassEntry = constantPool.get<ConstantPoolInfoClass>(thisClass);
                const JVM::u2 thisClassNameIndex = thisClassEntry->getNameIndex();
                throw std::runtime_error(fmt::format("Super Class not presented in \"{}\"", constantPool.get<ConstantPoolInfoUtf8>(thisClassNameIndex)->asString()));
            }

            return superClass.value();
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

        std::string getName() const
        {
            Java::JVM::u2 thisClassIndex = getThisClass();
            Java::JVM::u2 nameIndex = constantPool.get<Java::Archive::ConstantPoolInfoClass>(thisClassIndex)->getNameIndex();
            return constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(nameIndex)->asString();
        }

        bool isUnderPackage() const
        {
            return getName().find('/') != std::string::npos;
        }

        std::string getClassName() const
        {
            if (isUnderPackage())
            {
                std::vector<std::string> split = SuperJet::Utils::StringUtils::split(getName(), '/');
                return split[split.size() - 1];
            }

            return getName();
        }

    protected:
        JVM::u4 magic;
        JVM::u2 minorVersion;
        JVM::u2 majorVersion;
        ConstantPool constantPool;
        JVM::u2 accessFlags;
        JVM::u2 thisClass;
        std::optional<JVM::u2> superClass;
        std::vector<JVM::u2> interfaces;
        std::vector<FieldInfo> fields;
        std::vector<MethodInfo> methods;
        std::vector<AttributeInfo> attributes;
    };
}

namespace std
{
    template <>
    struct less<SuperJet::Java::Archive::ClassInfo>
    {
        bool operator()(const SuperJet::Java::Archive::ClassInfo& class1, const SuperJet::Java::Archive::ClassInfo& class2) const
        {
            const SuperJet::Java::Archive::ConstantPool& class1ConstantPool = class1.getConstantPool();
            SuperJet::Java::JVM::u2 class1Index = class1.getThisClass();
            SuperJet::Java::JVM::u2 class1NameIndex = class1ConstantPool.get<SuperJet::Java::Archive::ConstantPoolInfoClass>(class1Index)->getNameIndex();

            const SuperJet::Java::Archive::ConstantPool& class2ConstantPool = class2.getConstantPool();
            SuperJet::Java::JVM::u2 class2Index = class2.getThisClass();
            SuperJet::Java::JVM::u2 class2NameIndex = class2ConstantPool.get<SuperJet::Java::Archive::ConstantPoolInfoClass>(class2Index)->getNameIndex();

            return std::less<>()
            (
                    class1ConstantPool.get<SuperJet::Java::Archive::ConstantPoolInfoUtf8>(class1NameIndex)->asString(),
                    class2ConstantPool.get<SuperJet::Java::Archive::ConstantPoolInfoUtf8>(class2NameIndex)->asString()
            );
        }
    };
}


#endif //SUPERJET_CLASSINFO_H
