#ifndef LAV_H
#define LAV_H
#include <memory>
#include <string>
#include <vector>

#include "vec.h"
#include "util.h"

class SLAV;
class LAVertex;

class LAV {
public:
    shared_ptr<LAVertex> head;
    SLAV* slav;
    unsigned int len;
    LAV(SLAV& slav) : head(nullptr), slav(&slav), len(0) {
        cout << "Created slav " << this << endl;
    }

    static unique_ptr<LAV> from_polygon(const std::vector<Vec2>& polygon, SLAV& slav);
    static unique_ptr<LAV> from_chain(shared_ptr<LAVertex> chain_head, SLAV& slav);

    void invalidate(LAVertex& vertex);

    shared_ptr<LAVertex> unify(shared_ptr<LAVertex> vertex_a, shared_ptr<LAVertex> vertex_b, const Vec2& point);

    std::string toString() const;

    std::string toRepr() const;



    vector<shared_ptr<LAVertex>> toPolygon() const;
};

#endif // LAV_H