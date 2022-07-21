#ifndef SUPERJET_FIELDINFORESOLVER_H
#define SUPERJET_FIELDINFORESOLVER_H

#include "Compiler/Resolver.h"
#include "Compiler/AttributeInfoResolver.h"
#include "Java/Archive/ConstantPool.h"
#include "Java/Archive/FieldInfo.h"
#include "Java/Archive/FieldDescriptor.h"

namespace SuperJet::Compiler
{
    namespace ResolvedTypes
    {
        class FieldInfo
        {
        public:
            FieldInfo(const std::string& inName, ::SuperJet::Java::Archive::FieldInfo::AccessFlags inAccessFlags, const ::SuperJet::Java::Archive::FieldDescriptor& inDescriptor, const std::vector<ResolvedTypes::AttributeInfo>& inAttributes) : name(inName), accessFlags(inAccessFlags), descriptor(inDescriptor), attributes(inAttributes)
            {
            }

            const std::string& getName() const
            {
                return name;
            }

            ::SuperJet::Java::Archive::FieldInfo::AccessFlags getAccessFlags() const
            {
                return accessFlags;
            }

            ::SuperJet::Java::Archive::FieldDescriptor getDescriptor() const
            {
                return descriptor;
            }

            const std::vector<ResolvedTypes::AttributeInfo>& getAttributes() const
            {
                return attributes;
            }

        protected:
            std::string name;
            ::SuperJet::Java::Archive::FieldInfo::AccessFlags accessFlags;
            ::SuperJet::Java::Archive::FieldDescriptor descriptor;
            std::vector<ResolvedTypes::AttributeInfo> attributes;
        };
    }

    class FieldInfoResolver : Resolver<::SuperJet::Java::Archive::FieldInfo, ResolvedTypes::FieldInfo>
    {
    public:
        FieldInfoResolver(const ::SuperJet::Java::Archive::FieldInfo& fieldInfo, const ::SuperJet::Java::Archive::ConstantPool& inConstantPool) : Resolver<::SuperJet::Java::Archive::FieldInfo, ResolvedTypes::FieldInfo>(fieldInfo), constantPool(inConstantPool)
        {
        }

        virtual ResolvedTypes::FieldInfo resolve() const override;

    protected:
        ::SuperJet::Java::Archive::ConstantPool constantPool;
    };
}

#endif //SUPERJET_FIELDINFORESOLVER_H
