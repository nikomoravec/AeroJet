#include "Compiler/MethodInfoResolver.h"
#include "Java/Archive/ConstantPoolEntry.h"

namespace SuperJet::Compiler
{

    ResolvedTypes::MethodInfo MethodInfoResolver::resolve() const
    {
        const Java::Archive::MethodInfo::AccessFlags& accessFlags = resolvingObject.getAccessFlags();
        const Java::JVM::u2& methodNameIndex = resolvingObject.getNameIndex();
        const std::string& methodName = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(methodNameIndex)->asString();
        const Java::JVM::u2& descriptorIndex = resolvingObject.getDescriptorIndex();
        const Java::Archive::MethodDescriptor& methodDescriptor = Java::Archive::MethodDescriptor(constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(descriptorIndex)->asString());

        std::vector<ResolvedTypes::AttributeInfo> resolvedAttributeInfos;
        for (const auto& attributeInfo : resolvingObject.getAttributes())
        {
            AttributeInfoResolver attributeInfoResolver(attributeInfo, constantPool);
            resolvedAttributeInfos.emplace_back(attributeInfoResolver.resolve());
        }

        return ResolvedTypes::MethodInfo {accessFlags, methodName, methodDescriptor, resolvedAttributeInfos};
    }
}