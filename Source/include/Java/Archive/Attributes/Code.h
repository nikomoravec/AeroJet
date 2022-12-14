#ifndef SUPERJET_CODE_H
#define SUPERJET_CODE_H

#include "Java/Types.h"
#include "Java/Archive/AttributeInfo.h"
#include "Java/Archive/ConstantPool.h"
#include "Java/JVM/Runtime/Instruction.h"
#include "Java/JVM/Runtime/Operations.h"
#include <string>
#include <vector>

namespace SuperJet::Java::Archive::Attributes
{
    class Code
    {
    public:
        static constexpr auto CODE_ATTRIBUTE_NAME = "Code";

        class ExceptionTableEntry
        {
        public:
            ExceptionTableEntry(const JVM::u2 inStartPc, JVM::u2 inEndPc, const JVM::u2 inHandlePc, const JVM::u2 inCatchType) : startPc(inStartPc), endPc(inEndPc), handlerPc(inHandlePc), catchType(inCatchType)
            {
            }

            JVM::u2 getStartPc() const
            {
                return startPc;
            }

            JVM::u2 getEndPc() const
            {
                return endPc;
            }

            JVM::u2 getHandlerPc() const
            {
                return handlerPc;
            }

            JVM::u2 getCatchType() const
            {
                return catchType;
            }

        protected:
            JVM::u2 startPc;
            JVM::u2 endPc;
            JVM::u2 handlerPc;
            JVM::u2 catchType;
        };

    public:
        Code(const ConstantPool& constantPool, const AttributeInfo& attributeInfo);

        JVM::u2 getAttributeNameIndex() const
        {
            return attributeNameIndex;
        }

        JVM::u4 getAttributeLength() const
        {
            return attributeLength;
        }

        JVM::u2 getMaxStack() const
        {
            return maxStack;
        }

        JVM::u2 getMaxLocals() const
        {
            return maxLocals;
        }

        const std::vector<Java::JVM::Runtime::Instruction>& getCode() const
        {
            return code;
        }

        const std::vector<Code::ExceptionTableEntry>& getExceptionTable() const
        {
            return exceptionTable;
        }

        const std::vector<Archive::AttributeInfo>& getAttributes() const
        {
            return attributes;
        }

    protected:
        JVM::u2 attributeNameIndex;
        JVM::u4 attributeLength;
        JVM::u2 maxStack;
        JVM::u2 maxLocals;
        std::vector<Java::JVM::Runtime::Instruction> code;
        std::vector<ExceptionTableEntry> exceptionTable;
        std::vector<Archive::AttributeInfo> attributes;
    };
}

#endif //SUPERJET_CODE_H
