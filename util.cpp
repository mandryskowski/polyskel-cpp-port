#include "util.h"
#include "vec.h"

std::vector<Vec2> normalize_contour(const std::vector<Vec2>& contour) {
    std::vector<Vec2> points;
    for (const auto& vec : contour) {
        points.emplace_back(vec);
    }

    auto windowed = window<Vec2>(points);
    std::vector<Vec2> result;

    for (const auto& [prev, point, next] : windowed) {
        if (!(point == next || (point - prev).normalized() == (next - point).normalized())) {
            result.push_back(point);
        }
    }

    return result;
}

unique_ptr<Vec2> intersect(const Edge& l1, const Edge& l2, char type1, char type2) {
    Vec2 p1 = l1.start, q1 = l2.start;
    Vec2 r = l1.toVector(), s = l2.toVector();
    double r_cross_s = r.cross(s);
    // Parallel
    if (r_cross_s == 0)
        return nullptr;

    Vec2 q1_p1 = q1 - p1;
    double q1_p1_cross_r = q1_p1.cross(r);
    double q1_p1_cross_s = q1_p1.cross(s);
    double t = q1_p1_cross_s / r_cross_s;
    double u = q1_p1_cross_r / r_cross_s;
    double x = p1.x + t * r.x, y = p1.y + t * r.y;
    if (type1 == 's') {
        if (t < 0 || t > 1)
            return nullptr;
    } else if (type1 == 'r') {
        if (t < 0)
            return nullptr;
    }
    if (type2 == 's') {
        if (u < 0 || u > 1)
            return nullptr;
    } else if (type2 == 'r') {
        if (u < 0)
            return nullptr;
    }

    return make_unique<Vec2>(x, y);


}

Edge get_creator_vectors(const Edge& e_left, const Edge& e_right) {
    return Edge(e_left.toVector().normalized() * -1, e_right.toVector().normalized());
}

bool approximately_equals(double lhs, double rhs) {
    return lhs == rhs || (abs(lhs - rhs) <= max(abs(lhs), abs(rhs)) * 0.001);
}