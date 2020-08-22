#pragma once

#include <string>
#include <unordered_map>

#include "blmesh/common.h"
#include "blmesh/rw_lock.h"

namespace blmesh {

class ServiceManager {
 public:
  static ServiceManager &Get() {
    static ServiceManager ins;
    return ins;
  }

  void Register(const std::string &name, Port port);
  void Deregister(const std::string &name);
  Ptr<Port> Query(const std::string &name);
  bool Contains(const std::string &name);

  void Register_UNSAFE(const std::string &name, Port port);
  void Deregister_UNSAFE(const std::string &name);
  Ptr<Port> Query_UNSAFE(const std::string &name);
  bool Contains_UNSAFE(const std::string &name);

  template <typename Callable>
  void Iterate_UNSAFE(Callable f) {
    for (auto i : services_) {
      f(i.first, i.second);
    }
  }

  int GetSize() {
    auto g = ReadLockGuard{lock_};
    return services_.size();
  }

  int GetSize_UNSAFE() { return services_.size(); }

  RWLock &GetLock_UNSAFE() { return lock_; }

  ServiceManager(const ServiceManager &rhs) = delete;

 private:
  ServiceManager() = default;

  RWLock lock_;
  std::unordered_map<std::string, Port> services_;
};

}  // namespace blmesh