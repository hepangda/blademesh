//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include "ice/service_manager.h"

#include <random>

using blmesh::ReadLockGuard;
using blmesh::WriteLockGuard;

namespace blmesh {

void ServiceManager::Register(const std::string &name, const std::string &ip) {
  auto g = WriteLockGuard{lock_};
  srv_name_to_ip_[name].emplace(ip);
  srv_ip_to_name_[ip].emplace(name);
}

void ServiceManager::Deregister(const std::string &name, const std::string &ip) {
  if (!Contains(name)) {
    return;
  }

  auto g = WriteLockGuard{lock_};

  srv_name_to_ip_[name].erase(ip);
  if (srv_name_to_ip_[name].empty()) {
    srv_name_to_ip_.erase(name);
  }

  srv_ip_to_name_[ip].erase(name);
  if (srv_ip_to_name_[ip].empty()) {
    srv_ip_to_name_.erase(ip);
  }
}

Ptr<std::string> ServiceManager::Query(const std::string &name) {
  if (!Contains(name)) {
    return nullptr;
  }

  auto g = ReadLockGuard{lock_};

  std::random_device r;
  std::default_random_engine e{r()};
  std::uniform_int_distribution<int> uniform_dist(0, srv_name_to_ip_[name].size() - 1);

  auto iter = srv_name_to_ip_[name].cbegin();
  for (int i = 0; i < uniform_dist(e); i++) {
    iter++;
  }
  return std::make_unique<std::string>(*iter);
}

bool ServiceManager::Contains(const std::string &name) {
  auto g = ReadLockGuard{lock_};
  return srv_name_to_ip_.find(name) != srv_name_to_ip_.end();
}

int ServiceManager::GetSrvN() {
  auto g = ReadLockGuard{lock_};
  return srv_name_to_ip_.size();
}

std::vector<std::string> ServiceManager::GetSrv() {
  auto r = std::vector<std::string>{};
  auto g = ReadLockGuard{lock_};

  for (auto &i : srv_name_to_ip_) {
    r.emplace_back(i.first);
  }

  return r;
}

}  // namespace blmesh