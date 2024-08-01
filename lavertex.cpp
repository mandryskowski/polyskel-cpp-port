#include "lavertex.h"
#include "slav.h"
#include "lav.h"
#include "event.h"
#include "vec.h"

shared_ptr<Event> LAVertex::nextEvent() {
        vector<shared_ptr<Event>> events;
        if (is_reflex) {
            cout << "looking for split candidates for vertex " << toRepr() << endl;
            for (const auto& edge : lav->slav->original_edges) {
                if (edge->edge.approximately_same(edge_left) || edge->edge.approximately_same(edge_right)) {
                    cout << "\t NOT CONSIDERED EDGE " << edge->edge << endl;
                    continue;
                }

                cout << "\t considering EDGE " << edge->edge << endl;

                double leftdot = abs(edge_left.toVector().normalized().dot(edge->edge.toVector().normalized()));
                double rightdot = abs(edge_right.toVector().normalized().dot(edge->edge.toVector().normalized()));
                Edge selfedge = leftdot < rightdot ? edge_left : edge_right;

                unique_ptr<Vec2> i = intersect(selfedge, edge->edge, 'l', 's');
                if (i && !i->approximately_same(point)) {
                    Vec2 linvec = (point - *i).normalized();
                    Vec2 edvec = edge->edge.toVector().normalized();

                    if (linvec.dot(edvec) < 0)
                        edvec = Vec2(0,0) - edvec;
                    
                    Vec2 bisecvec = edvec + linvec;
                    if (bisecvec.length() == 0) {
                        continue;
                    }

                    Edge bisector = Edge(*i, *i + bisecvec);
                    unique_ptr<Vec2> b = intersect(bisector, this->bisector, 'r', 'r');

                    if (!b)
                        continue;
                    
                    bool xleft = edge->bisector_left.toVector().normalized().cross((*b - edge->bisector_left.start).normalized()) > -EPSILON;
                    bool xright = edge->bisector_right.toVector().normalized().cross((*b - edge->bisector_right.start).normalized()) < EPSILON;
                    bool xedge = edge->edge.toVector().normalized().cross((*b - edge->edge.start).normalized()) < EPSILON;

                    if (!(xleft && xright && xedge)) {
                        cout << "\t\tDiscarded candidate " << *b << " " << xleft << " " << xright << " " << xedge << endl;  
                        continue;
                    }

                    cout << "\t\tFound valid candidate " << *b << endl;
                    events.push_back(make_shared<SplitEvent>(edge->edge.distance_to(*b), *b, shared_from_this(), edge->edge));
                }


            }
        }

        unique_ptr<Vec2> i_prev = intersect(bisector, prev->bisector, 'r' , 'r');
        unique_ptr<Vec2> i_next = intersect(bisector, next->bisector, 'r' , 'r');

        if (i_prev)
            events.push_back(make_shared<EdgeEvent>(edge_left.distance_to(*i_prev), *i_prev, prev, shared_from_this()));
        if (i_next)
            events.push_back(make_shared<EdgeEvent>(edge_right.distance_to(*i_next), *i_next, shared_from_this(), next));

        if (events.empty()) {
            return nullptr;
        }

        shared_ptr<Event> ev = *min_element(events.begin(), events.end(), [this](const shared_ptr<Event>& a, const shared_ptr<Event>& b) {
            return (this->point - a->intersection_point).length() < (this->point - b->intersection_point).length();
        });

        cout << "Generated new event for " << toRepr() << " " << ev->toString() << endl;
        return ev;
    }

void LAVertex::invalidate() {
    if (lav) {
        lav->invalidate(*this);
    } else {
        valid = false;
    }
}

std::string LAVertex::toString() const {
    std::ostringstream os;
    os << "Vertex (" << point.x << "; " << point.y << ")";
    return os.str();
}

std::string LAVertex::toRepr() const {
    std::ostringstream os;
    os << "Vertex (" << (is_reflex ? "reflex" : "convex") << ") (" << point.x << "; " << point.y << ")";
    return os.str();
}