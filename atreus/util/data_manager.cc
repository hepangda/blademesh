#include "data_manager.h"

#include "blade/blade_object.h"

namespace blmesh {

void DataManager::DoQuery(const std::string &name) {
  auto g = WriteLockGuard{lock_};
  querys_[name]++;
}

void DataManager::DoError(const std::string &name) {
  auto g = WriteLockGuard{lock_};
  errors_[name]++;
}

void DataManager::DoShutdown(const std::string &name) {
  auto g = WriteLockGuard{lock_};
  shutdowns_[name]++;
}

void DataManager::DoBytes(const std::string &name, int bytes) {
  auto g = WriteLockGuard{lock_};
  bytes_[name] += bytes;
  bytes_[name] /= 2;
}

Ptr<BladeObject> DataManager::QueryReport() {
  auto ret = BladeObject::NewRequest(kWatchdogServiceName, kChaosReport);
  {
    auto g = WriteLockGuard{lock_};
    for (auto &i : querys_) {
      ret->AddI32(i.first, i.second);
    }
    querys_.clear();
  }

  ret->AddI64(kChaosReportType, 1);
  return ret;
}

Ptr<BladeObject> DataManager::ErrorReport() {
  auto ret = BladeObject::NewRequest(kWatchdogServiceName, kChaosReport);
  {
    auto g = WriteLockGuard{lock_};
    for (auto &i : errors_) {
      ret->AddI32(i.first, i.second);
    }
    errors_.clear();
  }

  ret->AddI64(kChaosReportType, 2);
  return ret;
}

Ptr<BladeObject> DataManager::ShutdownReport() {
  auto ret = BladeObject::NewRequest(kWatchdogServiceName, kChaosReport);
  {
    auto g = WriteLockGuard{lock_};
    for (auto &i : shutdowns_) {
      ret->AddI32(i.first, i.second);
    }
    shutdowns_.clear();
  }

  ret->AddI64(kChaosReportType, 3);
  return ret;
}

Ptr<BladeObject> DataManager::BytesReport() {
  auto ret = BladeObject::NewRequest(kWatchdogServiceName, kChaosReport);
  {
    auto g = WriteLockGuard{lock_};
    for (auto &i : bytes_) {
      ret->AddI32(i.first, i.second);
    }
    bytes_.clear();
  }

  ret->AddI64(kChaosReportType, 4);
  return ret;
}

}  // namespace blmesh
