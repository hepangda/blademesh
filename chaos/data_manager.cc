//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include "chaos/data_manager.h"

#include <algorithm>

using blmesh::WriteLockGuard;

namespace blmesh {

void DataManager::DoQuery(const std::string &name, int times) {
  auto g = WriteLockGuard{lock_};
  querys_[name] += times;
}

void DataManager::DoError(const std::string &name, int times) {
  auto g = WriteLockGuard{lock_};
  errors_[name] += times;
}

void DataManager::DoShutdown(const std::string &name, int times) {
  auto g = WriteLockGuard{lock_};
  shutdowns_[name] += times;
}

void DataManager::DoBytes(const std::string &name, int bytes) {
  auto g = WriteLockGuard{lock_};
  bytes_[name] += bytes;
  bytes_[name] /= 2;
}

int DataManager::GetQuery(const std::string &name) {
  auto g = ReadLockGuard{lock_};
  return querys_.at(name);
}

int DataManager::GetError(const std::string &name) {
  auto g = ReadLockGuard{lock_};
  return errors_.at(name);
}

int DataManager::GetShutdown(const std::string &name) {
  auto g = ReadLockGuard{lock_};
  return shutdowns_.at(name);
}

int DataManager::GetBytes(const std::string &name) {
  auto g = ReadLockGuard{lock_};
  return bytes_.at(name);
}

DataManager::MostResPtr DataManager::GetMostQuery() {
  auto g = ReadLockGuard{lock_};
  if (querys_.empty()) {
    return nullptr;
  }

  return std::make_unique<MostRes>(
      *std::max_element(querys_.begin(), querys_.end(), [](auto i, auto j) { return i.second < j.second; }));
}

DataManager::MostResPtr DataManager::GetMostError() {
  auto g = ReadLockGuard{lock_};
  if (errors_.empty()) {
    return nullptr;
  }

  return std::make_unique<MostRes>(
      *std::max_element(errors_.begin(), errors_.end(), [](auto i, auto j) { return i.second < j.second; }));
}

DataManager::MostResPtr DataManager::GetMostShutdown() {
  auto g = ReadLockGuard{lock_};
  if (shutdowns_.empty()) {
    return nullptr;
  }

  return std::make_unique<MostRes>(
      *std::max_element(shutdowns_.begin(), shutdowns_.end(), [](auto i, auto j) { return i.second < j.second; }));
}

}  // namespace blmesh