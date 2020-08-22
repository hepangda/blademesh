#pragma once

#include "blade/blade_server.h"

namespace blmesh {

class Kartos {
 public:
  Kartos();
  void Start();

 private:
  BladeServer server_;
};

}  // namespace blmesh
