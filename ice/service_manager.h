//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "blmesh/common.h"
#include "blmesh/rw_lock.h"

namespace blmesh {

class ServiceManager {
 public:
  static ServiceManager &Get() {
    static ServiceManager local_instance;
    return local_instance;
  }

  void Register(const std::string &name, const std::string &ip);
  void Deregister(const std::string &name, const std::string &ip);
  Ptr<std::string> Query(const std::string &name);
  bool Contains(const std::string &name);

  int GetSrvN();
  std::vector<std::string> GetSrv();

  ServiceManager(const ServiceManager &rhs) = delete;

 private:
  ServiceManager() = default;

  RWLock lock_;
  // name -> ip[]
  std::unordered_map<std::string, std::unordered_set<std::string>> srv_name_to_ip_;
  // ip -> name[]
  std::unordered_map<std::string, std::unordered_set<std::string>> srv_ip_to_name_;
};

}  // namespace blmesh