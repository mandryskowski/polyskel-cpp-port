#ifndef POLYSKEL_CPP_PORT_SLAV_H
#define POLYSKEL_CPP_PORT_SLAV_H

#include <memory>
#include <vector>
#include <utility>

#include "vec.h"
#include "event.h"

namespace polyskel {

    struct OriginalEdge;
    struct Subtree;
    class LAV;
    class Event;
    class EdgeEvent;
    class SplitEvent;

    class SLAV {
    public:
        std::vector<std::unique_ptr<LAV>> lavs;
        std::vector<std::shared_ptr<OriginalEdge>> original_edges;

        SLAV(const std::vector<std::vector<Vec2>>& polygons);

        std::vector<LAV*> toLavs();
        unsigned int length();
        bool empty();

        std::pair<std::shared_ptr<Subtree>, std::vector<std::shared_ptr<Event>>> handle_edge_event(EdgeEvent& event);
        std::pair<std::shared_ptr<Subtree>, std::vector<std::shared_ptr<Event>>> handle_split_event(SplitEvent& event);
    };;
}

#endif // POLYSKEL_CPP_PORT_SLAV_H