#ifndef POLYSKEL_H
#define POLYSKEL_H

#include <vector>
#include "vec.h"

using namespace std;

class Subtree;

vector<shared_ptr<Subtree>> skeletonize(vector<Vec2>& polygon, vector<vector<Vec2>>& holes);

#endif //POLYSKEL_H