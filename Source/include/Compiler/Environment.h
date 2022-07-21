#ifndef SUPERJET_ENVIRONMENT_H
#define SUPERJET_ENVIRONMENT_H

#include "Java/Archive/Jar.h"
#include <set>

namespace SuperJet::Compiler
{
    struct Environment
    {
        std::filesystem::path mainClass;
        std::set<std::string> libraries;
    };
}

#endif //SUPERJET_ENVIRONMENT_H
