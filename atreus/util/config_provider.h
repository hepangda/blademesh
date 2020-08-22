#pragma once

#include <string>

namespace blmesh {

class ConfigProvider {
 public:
  static void Init(int *argc, char **argv[]);
  static std::string GetThisIp();
  static std::string GetIceIp();
  static std::string GetChaosIp();
  static int GetKeepAliveDuration();
  static int GetReportDurtaion();
};

}  // namespace blmesh
