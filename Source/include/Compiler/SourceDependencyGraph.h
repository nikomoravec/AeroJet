#ifndef SUPERJET_SOURCEDEPENDENCYGRAPH_H
#define SUPERJET_SOURCEDEPENDENCYGRAPH_H

#include "Java/Archive/ClassInfo.h"
#include "dglib/digraph.hh"

namespace SuperJet::Compiler
{
    enum class ConnectionType : uint8_t
    {
        SOFT,
        HARD
    };

    using SourceDependencyGraph = digraph<Java::Archive::ClassInfo, ConnectionType>;
}

#endif //SUPERJET_SOURCEDEPENDENCYGRAPH_H
