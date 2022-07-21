#include "Java/Archive/ClassInfo.h"
#include "Java/Types.h"
#include "Compiler/Compiler.h"

template<>
std::string SuperJet::toString(const Java::Archive::ClassInfo& classInfo)
{
    const Java::Archive::ConstantPool& constanPool = classInfo.getConstantPool();
    Java::JVM::u2 thisClassIndex = classInfo.getThisClass();
    Java::JVM::u2 nameIndex = constanPool.get<Java::Archive::ConstantPoolInfoClass>(thisClassIndex)->getNameIndex();
    return constanPool.get<Java::Archive::ConstantPoolInfoUtf8>(nameIndex)->asString();
}