#ifndef SUPERJET_COMPILER_H
#define SUPERJET_COMPILER_H

#include "Compiler/Environment.h"
#include "Compiler/SourceDependencyGraph.h"

#include <filesystem>
#include <optional>
#include <map>
#include <unordered_map>

namespace SuperJet::Compiler
{
    class Context
    {
    public:
        Context(const Environment& env);

        const std::filesystem::path& getMainClass() const;
        std::shared_ptr<Java::Archive::Jar> jarForClass(const std::filesystem::path& javaClassName);

    protected:
        std::map<std::filesystem::path, std::shared_ptr<Java::Archive::Jar>> classMap;
        Environment environment;
    };

    class ByteCodeCompiler
    {
    public:
        ByteCodeCompiler(const Environment& env);
        void run();

    protected:
        Context context;
        SourceDependencyGraph dependencyGraph;
    };
}

#endif //SUPERJET_COMPILER_H
