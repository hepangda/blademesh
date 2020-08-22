#pragma once

#include "blade/blade_server.h"

namespace blmesh {

class Loki {
 public:
  Loki();
  void Start();

 private:
  BladeServer server_;
};

}  // namespace blmesh
