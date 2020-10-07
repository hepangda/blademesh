//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include "blade/blade_server.h"
#include "blade/service_session_policy.h"
#include "chaos/data_manager.h"
#include "chaos/handlers.h"

using namespace blmesh;

int main(int argc, char *argv[]) {
  auto policy = std::make_unique<ServiceSessionPolicy>(kIcePort);
  policy->DelegateExt(kChaosReport, DoReport);

  policy->DelegateExt(kChaosSrvQuery, DoQuery);
  policy->DelegateExt(kChaosSrvError, DoError);
  policy->DelegateExt(kChaosSrvShutdown, DoShutdown);
  policy->DelegateExt(kChaosSrvAvgBytes, DoBytes);

  policy->DelegateExt(kChaosMostQuery, DoMostQuery);
  policy->DelegateExt(kChaosMostError, DoMostError);
  policy->DelegateExt(kChaosMostShutdown, DoMostShutdown);

  auto server = std::make_shared<BladeServer>(kChaosPort, std::move(policy));
  server->Start();

  return 0;
}