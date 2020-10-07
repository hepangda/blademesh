//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include <string>
#include <unordered_map>

#include "blmesh/rw_lock.h"

namespace blmesh {

class DataManager {
 public:
  static DataManager &Get() {
    static DataManager ins;
    return ins;
  }

  void DoQuery(const std::string &name, int times);
  void DoError(const std::string &name, int times);
  void DoShutdown(const std::string &name, int times);
  void DoBytes(const std::string &name, int bytes);

  int GetQuery(const std::string &name);
  int GetError(const std::string &name);
  int GetShutdown(const std::string &name);
  int GetBytes(const std::string &name);

  using MostRes = std::pair<std::string, int>;
  using MostResPtr = std::unique_ptr<MostRes>;

  MostResPtr GetMostQuery();
  MostResPtr GetMostError();
  MostResPtr GetMostShutdown();

  DataManager(const DataManager &rhs) = delete;

 private:
  DataManager() = default;

  RWLock lock_;

  template <typename T>
  using CountStorage = std::unordered_map<std::string, T>;

  CountStorage<int> querys_;
  CountStorage<int> errors_;
  CountStorage<int> shutdowns_;
  CountStorage<int> bytes_;
};

}  // namespace blmesh
