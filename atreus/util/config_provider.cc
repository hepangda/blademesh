#include "config_provider.h"

#include <gflags/gflags.h>

#include "blmesh/common.h"

namespace blmesh {

static bool ValidateIp(const char *flag_name, const std::string &val) {
  try {
    nets::MakeAddress(val);
    return true;
  } catch (...) {
    printf("Invalid Format of %s, please check.\n", flag_name);
    return false;
  }
}

DEFINE_string(this_ip, "127.0.0.1", "this ip");
DEFINE_string(ice_ip, "127.0.0.1", "ice address");
DEFINE_string(chaos_ip, "127.0.0.1", "chaos ip");
DEFINE_validator(this_ip, ValidateIp);
DEFINE_validator(ice_ip, ValidateIp);
DEFINE_validator(chaos_ip, ValidateIp);
DEFINE_int32(kad, 10, "keep-alive duration");
DEFINE_int32(rpd, 10, "report duration");

void ConfigProvider::Init(int *argc, char ***argv) {
  gflags::ParseCommandLineFlags(argc, argv, true);
  gflags::SetUsageMessage("./atreus args...");
}

std::string ConfigProvider::GetThisIp() { return FLAGS_this_ip; }

std::string ConfigProvider::GetIceIp() { return FLAGS_ice_ip; }

int ConfigProvider::GetKeepAliveDuration() { return FLAGS_kad; }

int ConfigProvider::GetReportDurtaion() { return FLAGS_rpd; }

std::string ConfigProvider::GetChaosIp() { return FLAGS_chaos_ip; }

}  // namespace blmesh