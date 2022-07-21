#ifndef SUPERJET_METHODINFORESOLVER_H
#define SUPERJET_METHODINFORESOLVER_H

#include "Compiler/Resolver.h"
#include "Compiler/AttributeInfoResolver.h"

#include "Java/Archive/Attributes/Code.h"
#include "Java/Archive/Attributes/LineNumberTable.h"

#include "Java/Archive/ConstantPool.h"
#include "Java/Archive/MethodInfo.h"
#include "Java/Archive/MethodDescriptor.h"

#include "Java/Types.h"

#include <string>
#include <variant>
#include <vector>

namespace SuperJet::Compiler
{
    namespace ResolvedTypes
    {
        class MethodInfo
        {
        public:
            MethodInfo(::SuperJet::Java::Archive::MethodInfo::AccessFlags inAccessFlags, const std::string& inName, const ::SuperJet::Java::Archive::MethodDescriptor& inMethodDescriptor, const std::vector<ResolvedTypes::AttributeInfo>& inAttributes) : accessFlags(inAccessFlags), name(inName), descriptor(inMethodDescriptor), attributes(inAttributes)
            {
            }

            ::SuperJet::Java::Archive::MethodInfo::AccessFlags getAccessFlags() const
            {
                return accessFlags;
            }

            const std::string& getName() const
            {
                return name;
            }

            const ::SuperJet::Java::Archive::MethodDescriptor& getDescriptor() const
            {
                return descriptor;
            }

            const std::vector<ResolvedTypes::AttributeInfo>& getAttributes() const
            {
                return attributes;
            }

        protected:
            ::SuperJet::Java::Archive::MethodInfo::AccessFlags accessFlags;
            std::string name;
            ::SuperJet::Java::Archive::MethodDescriptor descriptor;
            std::vector<ResolvedTypes::AttributeInfo> attributes;
        };
    }


    class MethodInfoResolver : public Resolver<::SuperJet::Java::Archive::MethodInfo, ResolvedTypes::MethodInfo>
    {
    public:
        MethodInfoResolver(const ::SuperJet::Java::Archive::MethodInfo& methodInfo, const ::SuperJet::Java::Archive::ConstantPool& inConstantPool) : Resolver<::SuperJet::Java::Archive::MethodInfo, ResolvedTypes::MethodInfo>(methodInfo), constantPool(inConstantPool)
        {
        }

        virtual ResolvedTypes::MethodInfo resolve() const override;

    protected:
        ::SuperJet::Java::Archive::ConstantPool constantPool;
    };
}

#endif //SUPERJET_METHODINFORESOLVER_H
