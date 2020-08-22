//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include <thread>

#include "atreus/core/kartos.h"
#include "atreus/core/loki.h"
#include "atreus/core/watchdog.h"
#include "atreus/util/config_provider.h"
#include "blmesh/log.h"

using namespace blmesh;

int main(int argc, char *argv[]) {
  ConfigProvider::Init(&argc, &argv);

  blmesh::Log::Init(argv[0]);

  LogInfo("Atreus", "START") << std::endl;

  std::thread kartos_thread{[] { std::make_shared<Kartos>()->Start(); }};

  std::thread loki_thread{[] { std::make_shared<Loki>()->Start(); }};

  std::thread watchdog_thread{[] { std::make_shared<Watchdog>()->Start(); }};

  kartos_thread.join();
  loki_thread.join();
  watchdog_thread.join();

  blmesh::Log::Shutdown();
  return 0;
}