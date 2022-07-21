#ifndef SUPERJET_LOCALVARIABLETABLE_H
#define SUPERJET_LOCALVARIABLETABLE_H

#include "Java/Types.h"
#include "Java/Archive/AttributeInfo.h"
#include "Java/Archive/ConstantPool.h"

#include <vector>

namespace SuperJet::Java::Archive::Attributes
{
    class LocalVariableTable
    {
    public:
        static constexpr auto LOCAL_VARIABLE_TABLE_ATTRIBUTE_NAME = "LocalVariableTable";

        class LocalVariableTableEntry
        {
        public:
            LocalVariableTableEntry( JVM::u2 inStartPc, JVM::u2 inLength, JVM::u2 inNameIndex, JVM::u2 inDescriptorIndex, JVM::u2 inIndex) : startPc(inStartPc), length(inLength), nameIndex(inNameIndex), descriptorIndex(inDescriptorIndex), index(inIndex)
            {
            }

            JVM::u2 getStartPc() const
            {
                return startPc;
            }

            JVM::u2 getLength() const
            {
                return length;
            }

            JVM::u2 getNameIndex() const
            {
                return nameIndex;
            }

            JVM::u2 getDescriptorIndex() const
            {
                return descriptorIndex;
            }

            JVM::u2 getIndex() const
            {
                return index;
            }

        protected:
            JVM::u2 startPc;
            JVM::u2 length;
            JVM::u2 nameIndex;
            JVM::u2 descriptorIndex;
            JVM::u2 index;
        };

        LocalVariableTable(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        JVM::u2 getAttributeNameIndex() const
        {
            return attributeNameIndex;
        }

        JVM::u2 getAttributeLength() const
        {
            return attributeLength;
        }

        const std::vector<LocalVariableTableEntry>& getLocalVariableTable() const
        {
            return localVariableTable;
        }

    protected:
        JVM::u2 attributeNameIndex;
        JVM::u4 attributeLength;
        std::vector<LocalVariableTableEntry> localVariableTable;
    };
}

#endif //SUPERJET_LOCALVARIABLETABLE_H
