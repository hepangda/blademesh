#include "service_manager.h"

namespace blmesh {

void ServiceManager::Register(const std::string &name, Port port) {
  auto g = WriteLockGuard{lock_};
  services_.emplace(name, port);
}

void ServiceManager::Deregister(const std::string &name) {
  if (!Contains(name)) {
    return;
  }

  auto g = WriteLockGuard{lock_};
  services_.erase(name);
}

Ptr<Port> ServiceManager::Query(const std::string &name) {
  if (!Contains(name)) {
    return nullptr;
  }

  auto g = ReadLockGuard{lock_};
  return std::make_unique<Port>(services_[name]);
}

bool ServiceManager::Contains(const std::string &name) {
  auto g = ReadLockGuard{lock_};
  return services_.find(name) != services_.end();
}

void ServiceManager::Register_UNSAFE(const std::string &name, Port port) { services_.emplace(name, port); }

void ServiceManager::Deregister_UNSAFE(const std::string &name) {
  if (!Contains_UNSAFE(name)) {
    return;
  }

  services_.erase(name);
}

Ptr<Port> ServiceManager::Query_UNSAFE(const std::string &name) {
  if (!Contains_UNSAFE(name)) {
    return nullptr;
  }

  return std::make_unique<Port>(services_[name]);
}

bool ServiceManager::Contains_UNSAFE(const std::string &name) { return services_.find(name) != services_.end(); }

}  // namespace blmesh