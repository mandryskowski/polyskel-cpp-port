#include "slav.h"
#include "util.h"
#include "lav.h"
#include "lavertex.h"
#include "event.h"


SLAV::SLAV(const vector<vector<Vec2>>& polygons) {
    vector<vector<Vec2>> contours;
    for (const auto& polygon : polygons) {
        contours.push_back(normalize_contour(polygon));
    }

    for (const auto& contour : contours) {
        lavs.push_back(LAV::from_polygon(contour, *this));
    }

    for (auto& lav: lavs) {
        for (auto vert : lav->toPolygon()) {
            original_edges.push_back(make_shared<OriginalEdge>(Edge(vert->prev->point, vert->point), vert->prev->bisector, vert->bisector));
        }

    }
}

vector<LAV*> SLAV::toLavs() {
    vector<LAV*> result;
    for (auto& lav : lavs) {
        result.push_back(lav.get());
    }
    return result;
}
unsigned int SLAV::length() {
    return lavs.size();
}
bool SLAV::empty() {
    return lavs.empty();
}

pair<shared_ptr<Subtree>, vector<shared_ptr<Event>>> SLAV::handle_edge_event(EdgeEvent& event) {
    vector<Vec2> sinks;
    vector<shared_ptr<Event>> events;

    LAV* lav = event.vertex_a->lav;

    if (event.vertex_a->prev == event.vertex_b->next) {
        cout << event.distance << " Peak event at intersection " << event.intersection_point << " from " << event.vertex_a->toString() << " " << event.vertex_b->toString() << " " << event.vertex_a->prev->toString() << " in " << lav->toString() << endl;
        
        for (auto vert : lav->toPolygon()) {
            sinks.push_back(vert->point);
            vert->invalidate();
        }
        lavs.erase(std::remove_if(lavs.begin(), lavs.end(), [lav](const unique_ptr<LAV>& lav_ptr) { return lav_ptr.get() == lav; }), lavs.end());
    } else {
        cout << event.distance << " Edge event at intersection " << event.intersection_point << " from " << event.vertex_a->toString() << " " << event.vertex_b->toString() << " " << " in " << lav->toString() << endl;
        shared_ptr<LAVertex> new_vertex = lav->unify(event.vertex_a, event.vertex_b, event.intersection_point);
        if (lav->head == event.vertex_a || lav->head == event.vertex_b) {
            lav->head = new_vertex;
        }

        sinks.push_back(event.vertex_a->point);
        sinks.push_back(event.vertex_b->point);

        shared_ptr<Event> next_event = new_vertex->nextEvent();
        if (next_event) {
            events.push_back(next_event);
        }
    }

    return make_pair(make_shared<Subtree>(event.intersection_point, event.distance, sinks), events);
}

pair<shared_ptr<Subtree>, vector<shared_ptr<Event>>> SLAV::handle_split_event(SplitEvent& event) {
    LAV* lav = event.vertex->lav;
    vector<Vec2> sinks;
    sinks.push_back(event.vertex->point);

    cout << event.distance << " Split event at intersection " << event.intersection_point << " from vertex " << event.vertex->toString() << " for edge " << event.opposite_edge << " " << " in " << lav->toString() << endl;

    shared_ptr<LAVertex> x = nullptr; // right vertex
    shared_ptr<LAVertex> y = nullptr; // left vertex
    Vec2 normalized = event.opposite_edge.toVector().normalized();

    for (auto& lav: lavs) {
        for (auto v : lav->toPolygon()) {
            cout << v->toString() << " in " << v->lav->toString() << '\n';

            if (normalized == v->edge_left.toVector().normalized() && event.opposite_edge.start == v->edge_left.start) {
                x = v;
                y = x->prev;
            } else if (normalized == v->edge_right.toVector().normalized() && event.opposite_edge.start == v->edge_right.start) {
                y = v;
                x = y->next;
            }

            if (x) {
                bool xleft = y->bisector.toVector().normalized().cross((event.intersection_point - y->point).normalized()) >= -EPSILON;
                bool xright = x->bisector.toVector().normalized().cross((event.intersection_point - x->point).normalized()) <= EPSILON;

                if (xleft && xright)
                    break;
                else {
                    x = nullptr;
                    y = nullptr;
                }
            }
        }
    }

    if (!x) {
        cout << "Failed split event " << event.toString() << " (equivalent edge event is expected to follow)" << endl;
        return make_pair(nullptr, vector<shared_ptr<Event>>());
    }

    shared_ptr<LAVertex> v1 = make_shared<LAVertex>(event.intersection_point, event.vertex->edge_left, event.opposite_edge);
    shared_ptr<LAVertex> v2 = make_shared<LAVertex>(event.intersection_point, event.opposite_edge, event.vertex->edge_right);

    v1->prev = event.vertex->prev;
    v1->next = x;
    event.vertex->prev->next = v1;
    x->prev = v1;

    v2->prev = y;
    v2->next = event.vertex->next;
    event.vertex->next->prev = v2;
    y->next = v2;

    vector<unique_ptr<LAV>> new_lavs;
    lavs.erase(std::remove_if(lavs.begin(), lavs.end(), [lav](const unique_ptr<LAV>& lav_ptr) { return lav_ptr.get() == lav; }), lavs.end());

    if (lav != x->lav) {
        lavs.erase(std::remove_if(lavs.begin(), lavs.end(), [x](const unique_ptr<LAV>& lav_ptr) { return lav_ptr.get() == x->lav; }), lavs.end());
        new_lavs.push_back(LAV::from_chain(v1, *this));
    } else {
        new_lavs.push_back(LAV::from_chain(v1, *this));
        new_lavs.push_back(LAV::from_chain(v2, *this));
    }

    vector<shared_ptr<LAVertex>> vertices;

    for (auto& l: new_lavs) {
        if (l->len > 2) {
            vertices.push_back(l->head);
            lavs.push_back(std::move(l));
        } else {
            cout << "LAV " << l << " has collapsed into the line " << l->head->point << "--" << l->head->next->point << endl;
            sinks.push_back(l->head->next->point);
            for (auto vert : l->toPolygon()) {
                vert->invalidate();
            }
        }
    }

    vector<shared_ptr<Event>> events;
    for (auto& vert : vertices) {
        shared_ptr<Event> next_event = vert->nextEvent();
        if (next_event) {
            events.push_back(next_event);
        }
    }

    event.vertex->invalidate();
    return make_pair(make_shared<Subtree>(event.intersection_point, event.distance, sinks), events);
}