#ifndef SUPERJET_TRAITS_H
#define SUPERJET_TRAITS_H

#include "Compiler/Compiler.h"

#include "Java/Archive/Archive.h"
#include "Java/Archive/ClassInfo.h"
#include "Java/Archive/ConstantPool.h"
#include "Java/Types.h"

namespace SuperJet::Compiler::Type
{
    class Traits
    {
    public:
        static bool isBaseOf(const SuperJet::Compiler::Context& context, const std::shared_ptr<SuperJet::Java::Archive::ClassInfo>& classA, const std::shared_ptr<SuperJet::Java::Archive::ClassInfo>& classB)
        {
            if (!classB->isSuperClassPresented())
            {
                return false;
            }

            const SuperJet::Java::Archive::ConstantPool& classAConstantPool = classA->getConstantPool();
            SuperJet::Java::JVM::u2 classANameIndex = classAConstantPool.get<Java::Archive::ConstantPoolInfoClass>(classA->getThisClass())->getNameIndex();
            const std::string& classAName = classAConstantPool.get<Java::Archive::ConstantPoolInfoUtf8>(classANameIndex)->asString();

            const SuperJet::Java::Archive::ConstantPool& classBConstantPool = classB->getConstantPool();
            SuperJet::Java::JVM::u2 classBSuperClassNameIndex = classBConstantPool.get<Java::Archive::ConstantPoolInfoClass>(classB->getSuperClass())->getNameIndex();
            const std::string& classBSuperClassName = classBConstantPool.get<Java::Archive::ConstantPoolInfoUtf8>(classBSuperClassNameIndex)->asString();

            if (classBSuperClassName == classAName)
            {
                return true;
            }

            return isBaseOf(context, classA, Compiler::loadClass(context, classBSuperClassName));
        }
    };
}

#endif //SUPERJET_TRAITS_H
