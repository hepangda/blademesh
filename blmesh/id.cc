//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include "id.h"

#include <ctime>
#include <random>

namespace blmesh::id {

u32 Generate() {
  u32 result = time(nullptr) << 8;

  std::random_device r;
  std::default_random_engine e{r()};
  std::uniform_int_distribution<int> uniform_dist(0, 256);

  return result + uniform_dist(e);
}

}  // namespace blmesh::id