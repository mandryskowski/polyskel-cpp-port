#ifndef POLYSKEL_CPP_PORT_LAVERTEX_H
#define POLYSKEL_CPP_PORT_LAVERTEX_H
#include <vector>
#include <memory>
#include <iostream>

#include "vec.h"
#include "util.h"

namespace polyskel {

    class Event;
    class LAV;

    struct OriginalEdge {
        Edge edge;
        Edge bisector_left;
        Edge bisector_right;

        OriginalEdge(const Edge& edge, const Edge& bisector_left, const Edge& bisector_right)
            : edge(edge), bisector_left(bisector_left), bisector_right(bisector_right) {}
    };

    struct POLYSKEL_DLL_SYMBOL Subtree {
        Vec2 source;
        double height;
        std::vector<Vec2> sinks;
        
        Subtree(const Vec2& source, double height, const std::vector<Vec2>& sinks)
            : source(source), height(height), sinks(sinks) {}

        friend std::ostream& operator<<(std::ostream& os, const Subtree& point) {
            os << "Subtree from " << point.source << " with height " << point.height << " and sinks [";
            for (const Vec2& sink : point.sinks) {
                os << " " << sink;
            }
            os << " ]";
            return os;
        }
    };


    class LAVertex : public std::enable_shared_from_this<LAVertex> {
    public:
        Vec2 point;
        Edge edge_left;
        Edge edge_right;
        std::shared_ptr<LAVertex> prev;
        std::shared_ptr<LAVertex> next;
        Edge bisector;
        bool is_reflex;
        bool valid;

        LAV* lav;

        LAVertex(const Vec2& pt, const Edge& e_left, const Edge& e_right, const Edge& direction_vectors)
            : point(pt), edge_left(e_left), edge_right(e_right), prev(nullptr), next(nullptr), valid(true), bisector(Vec2(0,0), Vec2(0,0)), lav(nullptr) {
            // Additional initialization can go here
            Edge creator_vectors = get_creator_vectors(e_left, e_right);

            is_reflex = direction_vectors.start.cross(direction_vectors.end) < 0;
            bisector = Edge(point, point + (creator_vectors.start + creator_vectors.end) * (is_reflex ? -1 : 1));
        }
        LAVertex(const Vec2& pt, const Edge& e_left, const Edge& e_right):
            LAVertex(pt, e_left, e_right, get_creator_vectors(e_left, e_right)) {}

        std::shared_ptr<Event> nextEvent();

        void invalidate();

        std::string toString() const;

        std::string toRepr() const;
    };
}
#endif // POLYSKEL_CPP_PORT_LAVERTEX_H