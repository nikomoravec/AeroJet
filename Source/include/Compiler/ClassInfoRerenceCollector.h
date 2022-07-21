#ifndef SUPERJET_CLASSINFORERENCECOLLECTOR_H
#define SUPERJET_CLASSINFORERENCECOLLECTOR_H

#include "Compiler/Compiler.h"
#include "Visitor.h"
#include "SourceDependencyGraph.h"
#include "Java/Archive/ClassInfo.h"
#include "Java/Archive/Jar.h"

#include <filesystem>
#include <variant>
#include <map>
#include <unordered_set>

namespace SuperJet::Compiler
{
    class ClassInfoReferenceCollector final
    {
    public:
        ClassInfoReferenceCollector(const Context& ctx);
        SourceDependencyGraph collect(const std::filesystem::path& rootClass);
        SourceDependencyGraph collect(const Java::Archive::ClassInfo& rootClass);

    private:
        Context context;
        SourceDependencyGraph graph;
        std::set<Java::Archive::ClassInfo> visited;
    };




}

#endif //SUPERJET_CLASSINFORERENCECOLLECTOR_H
