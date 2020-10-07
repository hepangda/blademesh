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

DEFINE_string(ice_ip, "127.0.0.1", "ice ip");
DEFINE_validator(ice_ip, ValidateIp);

int main(int argc, char *argv[]) {
  gflags::SetUsageMessage("./get_most_errors -ice_ip <ice_ip>");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  cout << "!!!WARNING!!! This operation is DANGER, may have GREAT impact on performance" << endl
       << "You'd better run `get_service_number` first." << endl << endl
       << "Would you like to continue(Y/n): ";

  std::string res;
  cin >> res;
  if (res.empty() || toupper(res[0]) != 'Y') {
    return 0;
  }

  auto obj = BladeObject::NewRequest(kCallerServiceName, kChaosAvaSrv);

  try {
    auto res = SyncSession::CallExtNotSure(std::move(obj), kIcePort, FLAGS_ice_ip);
    if (res != nullptr && res->Contains(kChaosOk)) {
      cout << "\nFollowing service is available: " << endl;
      res->Iterate([](const BladeName &name, FieldType t, void *p) -> int {
        if (t == FieldType::Byte) {
          if (name != kChaosOk) {
            std::cout << name << std::endl;
          }
        }
        return 0;
      });
    } else {
      cout << "No data reported now" << endl;
    }
  } catch (...) {
    cout << "ICE doesn't available now." << endl;
  }

  return 0;
}
