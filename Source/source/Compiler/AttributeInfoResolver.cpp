#include "Compiler/AttributeInfoResolver.h"
#include "Compiler/Exceptions/NotSupportedException.h"
#include "fmt/format.h"

namespace SuperJet::Compiler
{

    ResolvedTypes::AttributeInfo AttributeInfoResolver::resolve() const
    {
        const Java::JVM::u2& attributeNameIndex = resolvingObject.getAttributeNameIndex();
        const std::string& attributeName = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(attributeNameIndex)->asString();

        if (attributeName == Java::Archive::Attributes::Code::CODE_ATTRIBUTE_NAME)
        {
            return {Java::Archive::Attributes::Code(constantPool, resolvingObject)};
        }

        if (attributeName == Java::Archive::Attributes::LineNumberTable::LINE_NUMBER_TABLE_ATTRIBUTE_NAME)
        {
            return {Java::Archive::Attributes::LineNumberTable(constantPool, resolvingObject)};
        }

        throw NotSupportedException(fmt::format("Attribute \"{}\" not supported!", attributeName));
    }
}
