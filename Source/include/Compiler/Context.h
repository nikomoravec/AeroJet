#ifndef SUPERJET_COMPILER_CONTEXT_H
#define SUPERJET_COMPILER_CONTEXT_H

#include "Compiler/Environment.h"
#include <map>

namespace SuperJet::Compiler
{
    class Context
    {
    public:
        Context(const Environment& env);

        const std::filesystem::path& getMainClass() const;
        std::shared_ptr<Java::Archive::Jar> jarForClass(const std::filesystem::path& javaClassName) const;

    protected:
        std::map<std::filesystem::path, std::shared_ptr<Java::Archive::Jar>> classMap;
        Environment environment;
    };
}

#endif //SUPERJET_COMPILER_CONTEXT_H