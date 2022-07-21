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
            const Java::Archive::Attributes::Code& attribute = Java::Archive::Attributes::Code(constantPool, resolvingObject);
            return {ResolvedTypes::Attributes::Code{constantPool, attribute}};
        }

        if (attributeName == Java::Archive::Attributes::LineNumberTable::LINE_NUMBER_TABLE_ATTRIBUTE_NAME)
        {
            const Java::Archive::Attributes::LineNumberTable& attribute = Java::Archive::Attributes::LineNumberTable(constantPool, resolvingObject);
            return {ResolvedTypes::Attributes::LineNumberTable{constantPool, attribute}};
        }

        if (attributeName == Java::Archive::Attributes::SourceFile::SOURCE_FILE_ATTRIBUTE_NAME)
        {
            const Java::Archive::Attributes::SourceFile& attribute = Java::Archive::Attributes::SourceFile(constantPool, resolvingObject);
            return {ResolvedTypes::Attributes::SourceFile{constantPool, attribute}};
        }

        throw NotSupportedException(fmt::format("Attribute \"{}\" not supported!", attributeName));
    }

    ResolvedTypes::Attributes::Code::Code(const Java::Archive::ConstantPool& constantPool, const Java::Archive::Attributes::Code& attribtute) : AttributeBase(attribtute.getAttributeNameIndex(), attribtute.getAttributeLength(), Java::Archive::Attributes::Code::CODE_ATTRIBUTE_NAME)
    {
        maxStack = attribtute.getMaxStack();
        maxLocals = attribtute.getMaxLocals();
        code = attribtute.getCode();
        exceptionTable = attribtute.getExceptionTable();

        for (const Java::Archive::AttributeInfo& attributeInfo : attribtute.getAttributes())
        {
            AttributeInfoResolver resolver(attributeInfo, constantPool);
            attributes.emplace_back(resolver.resolve());
        }
    }

    ResolvedTypes::Attributes::SourceFile::SourceFile(const Java::Archive::ConstantPool& constantPool, const Java::Archive::Attributes::SourceFile& attribtute) : AttributeBase(attribtute.getAttributeNameIndex(), attribtute.getAttributeLength(), Java::Archive::Attributes::SourceFile::SOURCE_FILE_ATTRIBUTE_NAME)
    {
        sourceFileName = constantPool.get<Java::Archive::ConstantPoolInfoUtf8>(attribtute.getSourceFileIndex())->asString();
    }

    ResolvedTypes::Attributes::LineNumberTable::LineNumberTable(const Java::Archive::ConstantPool& constantPool, const Java::Archive::Attributes::LineNumberTable& attribtute) : AttributeBase(attribtute.getAttributeNameIndex(), attribtute.getAttributeLength(), Java::Archive::Attributes::LineNumberTable::LINE_NUMBER_TABLE_ATTRIBUTE_NAME)
    {
        lineNumberTable = attribtute.getLineNumberTable();
    }
}
