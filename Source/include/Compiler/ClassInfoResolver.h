#ifndef SUPERJET_CLASSINFORESOLVER_H
#define SUPERJET_CLASSINFORESOLVER_H

#include "Compiler/Resolver.h"
#include "Compiler/FieldInfoResolver.h"
#include "Compiler/MethodInfoResolver.h"
#include "Compiler/AttributeInfoResolver.h"

#include "Java/Archive/ClassInfo.h"

#include <optional>
#include <string>

namespace SuperJet::Compiler
{
    namespace ResolvedTypes
    {
        using SoftClassInfoRef = std::string;

        class ClassInfo
        {
        public:
            ClassInfo(::SuperJet::Java::Archive::ClassInfo::AccessFlags inAccessFlags, const SoftClassInfoRef inThisClassRef, const std::optional<SoftClassInfoRef>& inSuperClassRef, const std::vector<SoftClassInfoRef>& inInterfaces, const std::vector<ResolvedTypes::FieldInfo>& inFileds, const std::vector<ResolvedTypes::MethodInfo>& inMethods, const std::vector<ResolvedTypes::AttributeInfo>& inAttributes) :
                accessFlags(inAccessFlags),
                thisClass(inThisClassRef),
                superClass(inSuperClassRef),
                interfaces(inInterfaces),
                fields(inFileds),
                methods(inMethods),
                attributes(inAttributes)
            {
            }

            ::SuperJet::Java::Archive::ClassInfo::AccessFlags getAccessFlags() const
            {
                return accessFlags;
            }

            const SoftClassInfoRef& getThisClass() const
            {
                return thisClass;
            }

            const std::optional<SoftClassInfoRef>& getSuperClass() const
            {
                return superClass;
            }

            const std::vector<SoftClassInfoRef> getInterfaces() const
            {
                return interfaces;
            }

            const std::vector<ResolvedTypes::FieldInfo>& getFields() const
            {
                return fields;
            }

            const std::vector<ResolvedTypes::MethodInfo>& getMethods() const
            {
                return methods;
            }

            const std::vector<ResolvedTypes::AttributeInfo>& getAttributes() const
            {
                return attributes;
            }

        protected:
            ::SuperJet::Java::Archive::ClassInfo::AccessFlags accessFlags;
            SoftClassInfoRef thisClass;
            std::optional<SoftClassInfoRef> superClass;
            std::vector<SoftClassInfoRef> interfaces;
            std::vector<ResolvedTypes::FieldInfo> fields;
            std::vector<ResolvedTypes::MethodInfo> methods;
            std::vector<ResolvedTypes::AttributeInfo> attributes;
        };
    }

    class ClassInfoResolver : public Resolver<::SuperJet::Java::Archive::ClassInfo, ResolvedTypes::ClassInfo>
    {
    public:
        ClassInfoResolver(const ::SuperJet::Java::Archive::ClassInfo& classInfo) : Resolver<::SuperJet::Java::Archive::ClassInfo, ResolvedTypes::ClassInfo>(classInfo)
        {
        }

        virtual ResolvedTypes::ClassInfo resolve() const override;
    };
}

#endif //SUPERJET_CLASSINFORESOLVER_H
