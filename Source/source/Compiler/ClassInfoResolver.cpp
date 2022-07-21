#include "Compiler/ClassInfoResolver.h"
#include "Compiler/FieldInfoResolver.h"
#include "Compiler/MethodInfoResolver.h"

namespace SuperJet::Compiler
{
    ResolvedTypes::ClassInfo ClassInfoResolver::resolve() const
    {
        const Java::Archive::ConstantPool& constantPool = resolvingObject.getConstantPool();
        const Java::Archive::ClassInfo::AccessFlags& accessFlags = resolvingObject.getAccessFlags();

        const Java::JVM::u2& thisClassIndex = resolvingObject.getThisClass();
        const Java::JVM::u2& thisClassNameIndex = constantPool.get<SuperJet::Java::Archive::ConstantPoolInfoClass>(thisClassIndex)->getNameIndex();
        ResolvedTypes::SoftClassInfoRef thisClassRef = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(thisClassNameIndex)->asString();

        std::optional<ResolvedTypes::SoftClassInfoRef> superClassRef {};
        if (resolvingObject.isSuperClassPresented())
        {
             const Java::JVM::u2& superClassIndex = resolvingObject.getSuperClass();
             const Java::JVM::u2& superClassNameIndex = constantPool.get<SuperJet::Java::Archive::ConstantPoolInfoClass>(superClassIndex)->getNameIndex();
             superClassRef = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(superClassNameIndex)->asString();
        }

        std::vector<ResolvedTypes::SoftClassInfoRef> interfaces;
        for (const Java::JVM::u2 interfaceIndex : resolvingObject.getInterfaces())
        {
            const Java::JVM::u2& interfaceNameIndex = constantPool.get<SuperJet::Java::Archive::ConstantPoolInfoClass>(interfaceIndex)->getNameIndex();
            ResolvedTypes::SoftClassInfoRef interfaceRef = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(interfaceNameIndex)->asString();

            interfaces.emplace_back(interfaceRef);
        }

        std::vector<ResolvedTypes::MethodInfo> methods;
        for (const Java::Archive::MethodInfo& methodInfo : resolvingObject.getMethods())
        {
            MethodInfoResolver methodInfoResolver(methodInfo, constantPool);
            methods.emplace_back(methodInfoResolver.resolve());
        }

        std::vector<ResolvedTypes::FieldInfo> fields;
        for (const Java::Archive::FieldInfo& fieldInfo : resolvingObject.getFields())
        {
            FieldInfoResolver fieldInfoResolver(fieldInfo, constantPool);
            fields.emplace_back(fieldInfoResolver.resolve());
        }

        std::vector<ResolvedTypes::AttributeInfo> attributes;
        for (const Java::Archive::AttributeInfo& attributeInfo : resolvingObject.getAttributes())
        {
            AttributeInfoResolver attributeInfoResolver(attributeInfo, constantPool);
            attributes.emplace_back(attributeInfoResolver.resolve());
        }

        return ResolvedTypes::ClassInfo{accessFlags, thisClassRef, superClassRef, interfaces, fields, methods, attributes};
    }
}