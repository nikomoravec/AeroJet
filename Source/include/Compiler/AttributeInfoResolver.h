#ifndef SUPERJET_ATTRIBUTERESOLVER_H
#define SUPERJET_ATTRIBUTERESOLVER_H

#include "Compiler/Resolver.h"

#include "Java/Archive/Attributes/Code.h"
#include "Java/Archive/Attributes/LineNumberTable.h"
#include "Java/Archive/Attributes/SourceFile.h"

#include "Java/Archive/AttributeInfo.h"
#include "Java/Archive/ConstantPool.h"

#include "Java/Types.h"

#include <variant>

namespace SuperJet::Compiler
{
    namespace ResolvedTypes
    {
        using AttributeInfo = std::variant
        <
                ::SuperJet::Java::Archive::Attributes::Code,
                ::SuperJet::Java::Archive::Attributes::LineNumberTable,
                ::SuperJet::Java::Archive::Attributes::SourceFile
        >;
    }

    class AttributeInfoResolver : public Resolver<::SuperJet::Java::Archive::AttributeInfo, ResolvedTypes::AttributeInfo>
    {
    public:
        AttributeInfoResolver(const ::SuperJet::Java::Archive::AttributeInfo& attributeInfo, const ::SuperJet::Java::Archive::ConstantPool& inConstantPool) : Resolver<::SuperJet::Java::Archive::AttributeInfo, ResolvedTypes::AttributeInfo>(attributeInfo), constantPool(inConstantPool)
        {
        }

        virtual ResolvedTypes::AttributeInfo resolve() const override;

    protected:
        ::SuperJet::Java::Archive::ConstantPool constantPool;
    };
}

#endif //SUPERJET_ATTRIBUTERESOLVER_H
