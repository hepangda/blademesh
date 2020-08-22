//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include <string>
#include <glog/logging.h>

namespace blmesh {

class Log {
 public:
  static void Init(const char *argv0);
  static void SetPrefix(const std::string &prefix);
  static void Shutdown();
  static void UpdateDest();
  static Log &Get();

  std::string GetLastEpoch();
  std::string GetEpoch();
  std::string Update();

  Log(const Log &rhs) = delete;
 private:
  Log();

  static std::string GenerateEpoch();
  void SetIPrefix(const std::string &prefix);
  std::string GeneratePrefix();

  std::string last_epoch_;
  std::string now_epoch_;
  std::string iprefix_;
};

#define LogInfo(name, type) LOG(INFO) << "[" << name << "] " << type << " "
#define LogError(name, type) LOG(ERROR) << "[" << name << "] " << type << " "
#define LogException(name) LOG(ERROR) << "[" << name << "] EXC "
#define LogItem(key, val) key << "=`" << val << "` "

}  // namespace blmesh