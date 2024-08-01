#include "lav.h"
#include "slav.h"
#include "lavertex.h"
#include <cassert>

unique_ptr<LAV> LAV::from_polygon(const std::vector<Vec2>& polygon, SLAV& slav) {
    auto lav = make_unique<LAV>(slav);

    auto windowed = window<Vec2>(polygon);
    std::vector<Vec2> result;

    for (const auto& [prev, point, next] : windowed) {
        lav->len += 1;
        auto vert = make_shared<LAVertex>(point, Edge(prev, point), Edge(point, next));
        vert->lav = lav.get();
        if (!lav->head) {
            lav->head = vert;
            vert->next = vert;
            vert->prev = vert;
        } else {
            vert->next = lav->head;
            vert->prev = lav->head->prev;
            vert->prev->next = vert;
            lav->head->prev = vert;
        }
    }

    return lav;
}
unique_ptr<LAV> LAV::from_chain(shared_ptr<LAVertex> chain_head, SLAV& slav) {
    auto lav = make_unique<LAV>(slav);

    lav->head = chain_head;

    for (auto vert : lav->toPolygon()) {
        lav->len += 1;
        vert->lav = lav.get();
    }

    return lav;
}

void LAV::invalidate(LAVertex& vertex) {
    assert(vertex.lav == this); // Cannot invalidate a vertex that isn't mine.

    cout << "Invalidating vertex " << vertex.toRepr() << endl;

    vertex.valid = false;

    if (&vertex == head.get()) {
        head = head->next;
    }

    vertex.lav = nullptr;
}

shared_ptr<LAVertex> LAV::unify(shared_ptr<LAVertex> vertex_a, shared_ptr<LAVertex> vertex_b, const Vec2& point) {
    shared_ptr<LAVertex> replacement = make_shared<LAVertex>(point, vertex_a->edge_left, vertex_b->edge_right, Edge(vertex_b->bisector.toVector().normalized(), vertex_a->bisector.toVector().normalized()));
    replacement->lav = this;

    if (head == vertex_a || head == vertex_b) {
        head = replacement;
    }

    vertex_a->prev->next = replacement;
    vertex_b->next->prev = replacement;
    replacement->prev = vertex_a->prev;
    replacement->next = vertex_b->next;

    vertex_a->invalidate();
    vertex_b->invalidate();

    len -= 1;
    return replacement;
}

std::string LAV::toString() const {
    std::ostringstream os;
    os << "LAV " << this;
    return os.str();
}

std::string LAV::toRepr() const {
    std::ostringstream os;
    os << toString() << " = [";
    for (auto vert : toPolygon()) {
        os << " " << vert->toRepr();
    }
    os << " ]";
    return os.str();
}



vector<shared_ptr<LAVertex>> LAV::toPolygon() const {
    if (!head) {
        return {};
    }

    vector<shared_ptr<LAVertex>> result;
    shared_ptr<LAVertex> current = head;
    do {
        result.push_back(current);
        current = current->next;
    } while (current != head);

    return result;
}