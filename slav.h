#ifndef SLAV_H
#define SLAV_H

#include <memory>
#include <vector>

#include "vec.h"

class LAV;
class OriginalEdge;
class Subtree;
class Event;
class EdgeEvent;
class SplitEvent;

class SLAV {
public:
    vector<unique_ptr<LAV>> lavs;
    vector<shared_ptr<OriginalEdge>> original_edges;

    SLAV(const vector<vector<Vec2>>& polygons);

    vector<LAV*> toLavs();
    unsigned int length();
    bool empty();

    pair<shared_ptr<Subtree>, vector<shared_ptr<Event>>> handle_edge_event(EdgeEvent& event);
    pair<shared_ptr<Subtree>, vector<shared_ptr<Event>>> handle_split_event(SplitEvent& event);
};


#endif //SLAV_H