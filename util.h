#ifndef UTIL_H
#define UTIL_H
#include <vector>
#include <tuple>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <string>
#include <sstream>

#define EPSILON 0.00001

class Vec2;
class Edge;

using namespace std;

template <typename T>
std::vector<std::tuple<T, T, T>> window(const std::vector<T>& lst) {
    std::vector<std::tuple<T, T, T>> result;
    size_t len = lst.size();

    if (len < 2) {
        return result;
    }

    for (size_t i = 0; i < len; ++i) {
        T prev = (i == 0) ? lst[len - 1] : lst[i - 1];
        T current = lst[i];
        T next = (i == len - 1) ? lst[0] : lst[i + 1];

        result.emplace_back(prev, current, next);
    }

    return result;
}

template <typename T>
std::vector<T> flatten(const std::vector<std::vector<T>>& nestedVec) {
    std::vector<T> flatVec;
    for (const auto& innerVec : nestedVec) {
        flatVec.insert(flatVec.end(), innerVec.begin(), innerVec.end());
    }
    return flatVec;
}

std::vector<Vec2> normalize_contour(const std::vector<Vec2>& contour);

unique_ptr<Vec2> intersect(const Edge& l1, const Edge& l2, char type1 = 'l', char type2 = 'l');

Edge get_creator_vectors(const Edge& e_left, const Edge& e_right);

bool approximately_equals(double lhs, double rhs);

#endif // UTIL_H