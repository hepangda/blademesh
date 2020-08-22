#pragma once

#include <string>
#include <unordered_map>

#include "blmesh/common.h"
#include "blmesh/rw_lock.h"

namespace blmesh {

class BladeObject;

class DataManager {
 public:
  static DataManager &Get() {
    static DataManager ins;
    return ins;
  }

  void DoQuery(const std::string &name);
  void DoError(const std::string &name);
  void DoShutdown(const std::string &name);
  void DoBytes(const std::string &name, int bytes);

  Ptr<BladeObject> QueryReport();
  Ptr<BladeObject> ErrorReport();
  Ptr<BladeObject> ShutdownReport();
  Ptr<BladeObject> BytesReport();

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