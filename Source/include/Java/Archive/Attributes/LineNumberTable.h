#ifndef SUPERJET_LINENUMBERTABLE_H
#define SUPERJET_LINENUMBERTABLE_H

#include "Java/Types.h"
#include "Java/Archive/AttributeInfo.h"
#include "Java/Archive/ConstantPool.h"
#include "Java/Archive/Archive.h"
#include "Compiler/Exceptions/IncorrectAttributeTypeException.h"
#include <vector>
#include <sstream>

namespace SuperJet::Java::Archive::Attributes
{
    class LineNumberTable
    {
    public:
        static constexpr auto LINE_NUMBER_TABLE_ATTRIBUTE_NAME = "LineNumberTable";

        class LineNUmberTableEntry
        {
        public:
            LineNUmberTableEntry(const JVM::u2 inStartPc, const JVM::u2 inLineNumber) : startPc(inStartPc), lineNumber(inLineNumber)
            {
            }

            JVM::u2 getStartPc() const
            {
                return startPc;
            }

            JVM::u2 getLineNumber() const
            {
                return lineNumber;
            }

        protected:
            JVM::u2 startPc;
            JVM::u2 lineNumber;
        };

    public:
        LineNumberTable(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        JVM::u2 getAttributeNameIndex() const
        {
            return attributeNameIndex;
        }

        JVM::u4 getAttributeLength() const
        {
            return attributeLength;
        }

        const std::vector<LineNUmberTableEntry>& getLineNumberTable() const
        {
            return lineNumberTable;
        }

    protected:
        JVM::u2 attributeNameIndex;
        JVM::u4 attributeLength;
        std::vector<LineNUmberTableEntry> lineNumberTable;
    };
}

#endif //SUPERJET_LINENUMBERTABLE_H
