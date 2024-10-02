#ifndef POLYSKEL_CPP_PORT_VEC_H
#define POLYSKEL_CPP_PORT_VEC_H
#include <cmath>
#include <iostream>
#include <ostream>
#include <algorithm>

#include "util.h"

namespace polyskel {

    struct POLYSKEL_DLL_SYMBOL Vec2 {
        double x;
        double y;

        Vec2(double x, double y): x(x), y(y) {}

        bool approximately_same(const Vec2& rhs) const { 
            return approximately_equals(x, rhs.x) && approximately_equals(y, rhs.y);
        }
        double dot(const Vec2& rhs) const {
            return x * rhs.x + y * rhs.y;
        }
        double cross(const Vec2& rhs) const {
            return x * rhs.y - y * rhs.x;
        }
        double length() const { 
            return std::sqrt(x * x + y * y);
        }
        Vec2 normalized() const {
            double len = length();
            return Vec2(x / len, y / len);
        }
        bool operator==(const Vec2& rhs) const {
            return x == rhs.x && y == rhs.y;
        }
        Vec2 operator-(const Vec2& rhs) const {
            return Vec2(x - rhs.x, y - rhs.y);
        }
        Vec2 operator+(const Vec2& rhs) const {
            return Vec2(x + rhs.x, y + rhs.y);
        }
        Vec2 operator*(double scalar) const {
            return Vec2(x * scalar, y * scalar);
        }
        friend std::ostream& operator<<(std::ostream& os, const Vec2& point) {
            os << "(" << point.x << ", " << point.y << ")";
            return os;
        }
    };

    struct Edge {
        Vec2 start;
        Vec2 end;

        Edge(Vec2 start, Vec2 end): start(start), end(end) {}

        Vec2 toVector() const {
            return end - start;
        }
        bool approximately_same(const Edge& rhs) const { 
            return start.approximately_same(rhs.start) && end.approximately_same(rhs.end);
        }
        double distance_to(const Vec2& vec) const {
            double length = toVector().length();
            if (length == 0) {
                return (start - vec).length();
            }
            return abs((end.x - start.x) * (start.y - vec.y) - (start.x - vec.x) * (end.y - start.y)) / length;
        }
        friend std::ostream& operator<<(std::ostream& os, const Edge& edge) {
            os << "(" << edge.start << ", " << edge.end << ")";
            return os;
        }
        bool operator==(const Edge& rhs) const {
            return start == rhs.start && end == rhs.end;
        }
    };

    struct vec2hash {
    public:
    std::size_t operator()(const Vec2 &vec) const
    {
        return std::hash<double>()(vec.x) ^ std::hash<double>()(vec.y);
    }
    };
}
#endif // POLYSKEL_CPP_PORT_VEC_H