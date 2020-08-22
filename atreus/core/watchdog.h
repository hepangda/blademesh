#pragma once

#include "blade/blade_server.h"

namespace blmesh {

class Watchdog {
 public:
  Watchdog() = default;
  void Start();

 private:
  static void Check();
  static void Report();

  BladeServer server_;
};

}  // namespace blmesh
