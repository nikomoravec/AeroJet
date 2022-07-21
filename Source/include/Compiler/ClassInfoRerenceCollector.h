#ifndef SUPERJET_CLASSINFORERENCECOLLECTOR_H
#define SUPERJET_CLASSINFORERENCECOLLECTOR_H

#include "Compiler/Environment.h"
#include "Visitor.h"
#include "SourceDependencyGraph.h"
#include "Java/Archive/ClassInfo.h"
#include "Java/Archive/Jar.h"

#include <filesystem>
#include <variant>
#include <map>
#include <unordered_set>

namespace std
{
    template <>
    struct less<SuperJet::Java::Archive::ClassInfo>
    {
        bool operator()(const SuperJet::Java::Archive::ClassInfo& class1, const SuperJet::Java::Archive::ClassInfo& class2) const
        {
            const SuperJet::Java::Archive::ConstantPool& class1ConstantPool = class1.getConstantPool();
            SuperJet::Java::JVM::u2 class1Index = class1.getThisClass();
            SuperJet::Java::JVM::u2 class1NameIndex = class1ConstantPool.get<SuperJet::Java::Archive::ConstantPoolInfoClass>(class1Index)->getNameIndex();

            const SuperJet::Java::Archive::ConstantPool& class2ConstantPool = class2.getConstantPool();
            SuperJet::Java::JVM::u2 class2Index = class2.getThisClass();
            SuperJet::Java::JVM::u2 class2NameIndex = class2ConstantPool.get<SuperJet::Java::Archive::ConstantPoolInfoClass>(class2Index)->getNameIndex();

            return std::less<>()
                    (
                            class1ConstantPool.get<SuperJet::Java::Archive::ConstantPoolInfoUtf8>(class1NameIndex)->asString(),
                            class2ConstantPool.get<SuperJet::Java::Archive::ConstantPoolInfoUtf8>(class2NameIndex)->asString()
                    );
        }
    };
}

namespace SuperJet::Compiler
{
    class ClassInfoReferenceCollector final
    {
    public:
        ClassInfoReferenceCollector(const Environment& compilerEnvironment);
        SourceDependencyGraph collect();

    protected:
        std::shared_ptr<Java::Archive::Jar> findJarForClass(const std::string& className);
        void processClass(const Java::Archive::ClassInfo& classInfo);

    private:
        const Environment& environment;
        std::map<std::filesystem::path, std::shared_ptr<Java::Archive::Jar>> classMap;
        SourceDependencyGraph graph;
        std::set<Java::Archive::ClassInfo> visited;
    };




}

#endif //SUPERJET_CLASSINFORERENCECOLLECTOR_H
