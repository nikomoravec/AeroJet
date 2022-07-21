#ifndef SUPERJET_ENVIRONMENT_H
#define SUPERJET_ENVIRONMENT_H

#include "Java/Archive/Jar.h"
#include <filesystem>
#include <vector>

namespace SuperJet::Compiler
{
    struct Environment
    {
        std::filesystem::path mainClass;
        std::vector<std::filesystem::path> libraries;
    };
}

#endif //SUPERJET_ENVIRONMENT_H
