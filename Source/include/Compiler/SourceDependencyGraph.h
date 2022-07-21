#ifndef SUPERJET_SOURCEDEPENDENCYGRAPH_H
#define SUPERJET_SOURCEDEPENDENCYGRAPH_H

#include "Java/Archive/ClassInfo.h"
#include "dglib/digraph.hh"
#include <string>
#include <queue>

namespace SuperJet::Compiler
{
    enum class ConnectionType : uint8_t
    {
        SOFT,
        HARD
    };

    class SourceDependencyGraph : public digraph<std::shared_ptr<Java::Archive::ClassInfo>, ConnectionType>
    {
    public:
        std::queue<std::shared_ptr<Java::Archive::ClassInfo>> topology()
        {
            std::set<std::shared_ptr<Java::Archive::ClassInfo>> visited;
            std::queue<std::shared_ptr<Java::Archive::ClassInfo>> result;

            for(const std::shared_ptr<Java::Archive::ClassInfo>& node : nodes())
            {
                if (!visited.contains(node))
                {
                    topologySort(node, visited, result);
                }
            }
            
            return result;
        }

    private:
        void topologySort(const std::shared_ptr<Java::Archive::ClassInfo>& node, std::set<std::shared_ptr<Java::Archive::ClassInfo>>& visited, std::queue<std::shared_ptr<Java::Archive::ClassInfo>>& result)
        {
            visited.emplace(node);

            const std::map<std::shared_ptr<Java::Archive::ClassInfo>, ConnectionType>& nodeConnections = connections(node);
            for (const auto& connection : nodeConnections)
            {
                ConnectionType connectionType = connection.second;
                if (connectionType == ConnectionType::HARD)
                {
                    if(!visited.contains(connection.first))
                    {
                        topologySort(connection.first, visited, result);
                    }
                }
            }

            result.push(node);
        }
    };
}

#endif //SUPERJET_SOURCEDEPENDENCYGRAPH_H
