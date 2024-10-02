#ifndef POLYSKEL_CPP_PORT_LAV_H
#define POLYSKEL_CPP_PORT_LAV_H
#include <memory>
#include <string>
#include <vector>

#include "vec.h"
#include "util.h"

namespace polyskel {

    class SLAV;
    class LAVertex;

    class LAV {
    public:
        std::shared_ptr<LAVertex> head;
        SLAV* slav;
        unsigned int len;
        LAV(SLAV& slav) : head(nullptr), slav(&slav), len(0) {
        }
        std::unique_ptr<LAV> clone() const;
        static std::unique_ptr<LAV> from_polygon(const std::vector<Vec2>& polygon, SLAV& slav);
        static std::unique_ptr<LAV> from_chain(std::shared_ptr<LAVertex> chain_head, SLAV& slav);

        void invalidate(LAVertex& vertex);

        std::shared_ptr<LAVertex> unify(std::shared_ptr<LAVertex> vertex_a, std::shared_ptr<LAVertex> vertex_b, const Vec2& point);

        std::string toString() const;

        std::string toRepr() const;

        std::vector<std::shared_ptr<LAVertex>> toPolygon() const;
    };
}
#endif // POLYSKEL_CPP_PORT_LAV_H