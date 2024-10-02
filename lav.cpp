#include "lav.h"
#include "slav.h"
#include "lavertex.h"
#include <cassert>

using namespace polyskel;

std::unique_ptr<LAV> polyskel::LAV::from_polygon(const std::vector<Vec2>& polygon, SLAV& slav) {
    auto lav = std::make_unique<LAV>(slav);

    auto windowed = window<Vec2>(polygon);
    std::vector<Vec2> result;

    for (const auto& [prev, point, next] : windowed) {
        lav->len += 1;
        auto vert = std::make_shared<LAVertex>(point, Edge(prev, point), Edge(point, next));
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
std::unique_ptr<LAV> polyskel::LAV::from_chain(std::shared_ptr<LAVertex> chain_head, SLAV& slav) {
    auto lav = std::make_unique<LAV>(slav);

    lav->head = chain_head;

    for (auto vert : lav->toPolygon()) {
        lav->len += 1;
        vert->lav = lav.get();
    }

    return lav;
}

void polyskel::LAV::invalidate(LAVertex& vertex) {
    assert(vertex.lav == this); // Cannot invalidate a vertex that isn't mine.

    vertex.valid = false;

    if (&vertex == head.get()) {
        head = head->next;
    }

    vertex.lav = nullptr;
}

std::shared_ptr<LAVertex> polyskel::LAV::unify(std::shared_ptr<LAVertex> vertex_a, std::shared_ptr<LAVertex> vertex_b, const Vec2& point) {
    std::shared_ptr<LAVertex> replacement = std::make_shared<LAVertex>(point, vertex_a->edge_left, vertex_b->edge_right, Edge(vertex_b->bisector.toVector().normalized(), vertex_a->bisector.toVector().normalized()));
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

std::string polyskel::LAV::toString() const {
    std::ostringstream os;
    os << "LAV " << this;
    return os.str();
}

std::string polyskel::LAV::toRepr() const {
    std::ostringstream os;
    os << toString() << " = [";
    for (auto vert : toPolygon()) {
        os << " " << vert->toRepr();
    }
    os << " ]";
    return os.str();
}



std::vector<std::shared_ptr<LAVertex>> polyskel::LAV::toPolygon() const {
    if (!head) {
        return {};
    }

    std::vector<std::shared_ptr<LAVertex>> result;
    std::shared_ptr<LAVertex> current = head;
    do {
        result.push_back(current);
        current = current->next;
    } while (current != head);

    return result;
}