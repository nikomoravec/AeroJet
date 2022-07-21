#ifndef SUPERJET_SOURCEDEPENDENCYGRAPH_H
#define SUPERJET_SOURCEDEPENDENCYGRAPH_H

#include "Java/Archive/ClassInfo.h"
#include "dglib/digraph.hh"
#include <string>

namespace SuperJet::Compiler
{
    enum class ConnectionType : uint8_t
    {
        SOFT,
        HARD
    };

    using SourceDependencyGraph = digraph<std::string, ConnectionType>;
}

#endif //SUPERJET_SOURCEDEPENDENCYGRAPH_H
