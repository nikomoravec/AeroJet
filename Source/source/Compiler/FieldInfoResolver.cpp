#include "Compiler/FieldInfoResolver.h"

namespace SuperJet::Compiler
{

    ResolvedTypes::FieldInfo FieldInfoResolver::resolve() const
    {
        Java::JVM::u2 fieldNameIndex = resolvingObject.getNameIndex();
        Java::JVM::u2 fieldDescriptorIndex = resolvingObject.getDescriptorIndex();
        Java::Archive::FieldInfo::AccessFlags fieldAccessFlags = resolvingObject.getAccessFlags();
        const std::vector<Java::Archive::AttributeInfo>& fieldAttributes = resolvingObject.getAttributes();

        const std::string& fieldName = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(fieldNameIndex)->asString();
        const Java::Archive::FieldDescriptor& fieldDescriptor = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(fieldDescriptorIndex)->asString();

        std::vector<ResolvedTypes::AttributeInfo> resolvedAttributes;
        resolvedAttributes.reserve(fieldAttributes.size());
        for (const Java::Archive::AttributeInfo& attribute : fieldAttributes)
        {
            AttributeInfoResolver attributeInfoResolver(attribute, constantPool);
            resolvedAttributes.emplace_back(attributeInfoResolver.resolve());
        }

        return ResolvedTypes::FieldInfo{fieldName, fieldAccessFlags, fieldDescriptor, resolvedAttributes};
    }
}