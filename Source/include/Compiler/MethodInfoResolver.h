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
            MethodInfo(Java::Archive::MethodInfo::AccessFlags inAccessFlags, const std::string& inName, const Java::Archive::MethodDescriptor& inMethodDescriptor, const std::vector<ResolvedTypes::AttributeInfo>& inAttributes) : accessFlags(inAccessFlags), name(inName), descriptor(inMethodDescriptor), attributes(inAttributes)
            {
            }

            Java::Archive::MethodInfo::AccessFlags getAccessFlags() const
            {
                return accessFlags;
            }

            const std::string& getName() const
            {
                return name;
            }

            const Java::Archive::MethodDescriptor& getDescriptor() const
            {
                return descriptor;
            }

            const std::vector<ResolvedTypes::AttributeInfo>& getAttributes() const
            {
                return attributes;
            }

        protected:
            Java::Archive::MethodInfo::AccessFlags accessFlags;
            std::string name;
            Java::Archive::MethodDescriptor descriptor;
            std::vector<ResolvedTypes::AttributeInfo> attributes;
        };
    }


    class MethodInfoResolver : public Resolver<Java::Archive::MethodInfo, ResolvedTypes::MethodInfo>
    {
    public:
        MethodInfoResolver(const Java::Archive::MethodInfo& methodInfo, const Java::Archive::ConstantPool& inConstantPool) : Resolver<Java::Archive::MethodInfo, ResolvedTypes::MethodInfo>(methodInfo), constantPool(inConstantPool)
        {
        }

        virtual ResolvedTypes::MethodInfo resolve() const override;

    protected:
        Java::Archive::ConstantPool constantPool;
    };
}

#endif //SUPERJET_METHODINFORESOLVER_H
