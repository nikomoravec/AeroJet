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
        namespace Attributes
        {
            class Code;
            class LineNumberTable;
            class SourceFile;
        }

        using AttributeInfo = std::variant
        <
                Attributes::Code,
                Attributes::LineNumberTable,
                Attributes::SourceFile
        >;

        namespace Attributes
        {
            namespace
            {
                class AttributeBase
                {
                public:
                    AttributeBase(Java::JVM::u2 inAttributeNameIndex, Java::JVM::u4 inAttributeLength, const std::string& inAttributeName) : attributeNameIndex(inAttributeNameIndex), attributeLength(inAttributeLength), attributeName(inAttributeName)
                    {
                    }

                    Java::JVM::u2 getAttributeNameIndex() const
                    {
                        return attributeNameIndex;
                    }

                    Java::JVM::u4 getAttributeLength() const
                    {
                        return attributeLength;
                    }

                    const std::string& getAttributeName() const
                    {
                        return attributeName;
                    }

                protected:
                    Java::JVM::u2 attributeNameIndex;
                    Java::JVM::u4 attributeLength;
                    std::string attributeName;
                };
            }

            class Code : public AttributeBase
            {
            public:
                Code(const Java::Archive::ConstantPool& constantPool, const Java::Archive::Attributes::Code& attribtute);

                Java::JVM::u2 getMaxStack() const
                {
                    return maxStack;
                }

                Java::JVM::u2 getMaxLocals() const
                {
                    return maxLocals;
                }

                const std::vector<std::shared_ptr<Java::JVM::Runtime::Operation>>& getCode() const
                {
                    return code;
                }

                const std::vector<::SuperJet::Java::Archive::Attributes::Code::ExceptionTableEntry>& getExceptionTable() const
                {
                    return exceptionTable;
                }

                const std::vector<ResolvedTypes::AttributeInfo>& getAttributes() const
                {
                    return attributes;
                }

            protected:
                Java::JVM::u2 maxStack;
                Java::JVM::u2 maxLocals;
                std::vector<std::shared_ptr<Java::JVM::Runtime::Operation>> code;
                std::vector<::SuperJet::Java::Archive::Attributes::Code::ExceptionTableEntry> exceptionTable;
                std::vector<ResolvedTypes::AttributeInfo> attributes;
            };

            class SourceFile : public AttributeBase
            {
            public:
                SourceFile(const Java::Archive::ConstantPool& constantPool, const Java::Archive::Attributes::SourceFile& attribtute);

            protected:
                std::string sourceFileName;
            };

            class LineNumberTable : public AttributeBase
            {
            public:
                LineNumberTable(const Java::Archive::ConstantPool& constantPool, const Java::Archive::Attributes::LineNumberTable& attribtute);

                const std::vector<Java::Archive::Attributes::LineNumberTable::LineNUmberTableEntry>& getLineNumberTable() const
                {
                    return lineNumberTable;
                }

            protected:
                std::vector<Java::Archive::Attributes::LineNumberTable::LineNUmberTableEntry> lineNumberTable;
            };
        }
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
