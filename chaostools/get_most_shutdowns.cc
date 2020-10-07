//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include <iostream>
#include <string>

#include "blade/blade_object.h"
#include "blade/sync_session.h"
#include "gflags/gflags.h"

using namespace std;
using namespace blmesh;

static bool ValidateIp(const char *flag_name, const std::string &val) {
  try {
    auto g = nets::MakeAddress(val);
    return true;
  } catch (...) {
    printf("Invalid Format of %s, please check.\n", flag_name);
    return false;
  }
}

DEFINE_string(chaos_ip, "127.0.0.1", "chaos ip");
DEFINE_validator(chaos_ip, ValidateIp);

int main(int argc, char *argv[]) {
  gflags::SetUsageMessage("./get_most_shutdowns -chaos_ip <chaos_ip>");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  auto obj = BladeObject::NewRequest(kCallerServiceName, kChaosMostShutdown);

  try {
    auto res = SyncSession::CallExtNotSure(std::move(obj), kChaosPort, FLAGS_chaos_ip);
    if (res != nullptr && res->Contains(kChaosOk)) {
      if (res->Contains(kNoAnswer)) {
        cout << "None service reported shutdowns." << std::endl;
      } else {
        auto name = res->GetString(kChaosName);
        auto val = res->GetI32(kChaosVal);
        cout << "Service[" << name << "] reported " << val << " shutdowns mostly." << std::endl;
      }
    } else {
      cout << "No data reported now" << endl;
    }
  } catch (...) {
    cout << "CHAOS doesn't available now." << endl;
  }

  return 0;
}