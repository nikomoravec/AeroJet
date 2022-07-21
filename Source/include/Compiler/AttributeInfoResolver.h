#ifndef SUPERJET_ATTRIBUTERESOLVER_H
#define SUPERJET_ATTRIBUTERESOLVER_H

#include "Compiler/Resolver.h"

#include "Java/Archive/Attributes/Code.h"
#include "Java/Archive/Attributes/LineNumberTable.h"

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
                Java::Archive::Attributes::Code,
                Java::Archive::Attributes::LineNumberTable
        >;
    }

    class AttributeInfoResolver : public Resolver<Java::Archive::AttributeInfo, ResolvedTypes::AttributeInfo>
    {
    public:
        AttributeInfoResolver(const Java::Archive::AttributeInfo& attributeInfo, const Java::Archive::ConstantPool& inConstantPool) : Resolver<Java::Archive::AttributeInfo, ResolvedTypes::AttributeInfo>(attributeInfo), constantPool(inConstantPool)
        {
        }

        virtual ResolvedTypes::AttributeInfo resolve() const override;

    protected:
        Java::Archive::ConstantPool constantPool;
    };
}

#endif //SUPERJET_ATTRIBUTERESOLVER_H
