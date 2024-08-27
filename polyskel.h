#ifndef POLYSKEL_H
#define POLYSKEL_H

#include <vector>
#include "util.h"
#include "vec.h"

namespace polyskel {

    struct Subtree;

    POLYSKEL_DLL_SYMBOL std::vector<std::shared_ptr<Subtree>> skeletonize(std::vector<Vec2>& polygon, std::vector<std::vector<Vec2>>& holes);
}
#endif //POLYSKEL_H