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
DEFINE_string(q, "", "query");
DEFINE_validator(chaos_ip, ValidateIp);

int main(int argc, char *argv[]) {
  gflags::SetUsageMessage("./get_service_shutdowns -chaos_ip <chaos_ip> -q <service name>");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  if (FLAGS_q.empty()) {
    cin >> FLAGS_q;
  }

  auto obj = BladeObject::NewRequest(kCallerServiceName, kChaosSrvShutdown);
  obj->AddString(kChaosName, FLAGS_q);

  try {
    auto res = SyncSession::CallExtNotSure(std::move(obj), kChaosPort, FLAGS_chaos_ip);
    if (res != nullptr && res->Contains(kChaosOk)) {
      auto val = res->GetI32(FLAGS_q);
      cout << "Service[" << FLAGS_q << "] reported " << val << " shutdowns." << std::endl;
    } else {
      cout << "No shutdowns reported in service[" << FLAGS_q << "]" << endl;
    }
  } catch (...) {
    cout << "CHAOS doesn't available now." << endl;
  }

  return 0;
}
