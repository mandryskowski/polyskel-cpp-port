#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <string>
#include <sstream>
#include <utility>
#include <optional>
#include <memory>
#include <cassert>
#include <unordered_map>

#include "vec.h"
#include "util.h"
#include "event.h"
#include "slav.h"
#include "lav.h"
#include "polyskel.h"


using namespace polyskel;

void _merge_sources(std::vector<std::shared_ptr<Subtree>>& skeleton) {
    std::unordered_map<Vec2, unsigned int, vec2hash> sources;
    std::vector<unsigned int> to_remove;

    for (unsigned int i = 0; i < skeleton.size(); i++) {
        Subtree& p = *skeleton[i]; 
        Vec2 source = p.source;
        if (sources.find(source) != sources.end()) {
            unsigned int source_index = sources[source];
            // source exists, merge sinks
            for (const Vec2& sink : p.sinks) {
                if (std::find(skeleton[source_index]->sinks.begin(), skeleton[source_index]->sinks.end(), sink) == skeleton[source_index]->sinks.end()) {
                    skeleton[source_index]->sinks.push_back(sink);
                }
            }
            to_remove.push_back(i);
        } else {
            sources[source] = i;
        }
    }

    for (int i = static_cast<int>(to_remove.size()) - 1; i >= 0; i--) {
        skeleton.erase(skeleton.begin() + to_remove[i]);
    }
}

std::vector<std::shared_ptr<Subtree>> polyskel::skeletonize(std::vector<Vec2>& polygon, std::vector<std::vector<Vec2>>& holes) {
    std::vector<std::vector<Vec2>> polygons;
    polygons.push_back(polygon);
    for (const auto& hole : holes) {
        polygons.push_back(hole);
    }

    auto slav = std::make_unique<SLAV>(polygons);
    std::vector<std::shared_ptr<Subtree>> output;
    EventQueue prioqueue;

    for (auto& lav: slav->lavs) {
        for (auto vert : lav->toPolygon()) {
            prioqueue.put(vert->nextEvent());
        }
    }

    while (!(prioqueue.empty() || slav->empty())) {
        std::shared_ptr<Event> i = prioqueue.get();

        std::shared_ptr<Subtree> arc = nullptr;
        std::vector<std::shared_ptr<Event>> events;

        if (auto edge_event = std::dynamic_pointer_cast<EdgeEvent>(i)) {
            if (!edge_event->vertex_a->valid || !edge_event->vertex_b->valid) {
                continue;
            }
            std::tie(arc, events) = slav->handle_edge_event(*edge_event);
        } else if (auto split_event = std::dynamic_pointer_cast<SplitEvent>(i)) {
            if (!split_event->vertex->valid) {
                continue;
            }
            std::tie(arc, events) = slav->handle_split_event(*split_event);
        }

        prioqueue.put_all(events);

        if (arc) {
            output.push_back(arc);
        }
    }

    _merge_sources(output);
    
    return output;
}
