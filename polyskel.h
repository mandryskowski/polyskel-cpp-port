#ifndef POLYSKEL_H
#define POLYSKEL_H

#include <vector>
#include "util.h"
#include "vec.h"

using namespace std;

struct Subtree;

POLYSKEL_DLL_SYMBOL vector<shared_ptr<Subtree>> skeletonize(vector<Vec2>& polygon, vector<vector<Vec2>>& holes);

#endif //POLYSKEL_H