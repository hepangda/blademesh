//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include <mutex>
#include <condition_variable>

#include "blmesh/noncopyable.h"

namespace blmesh {

class RWLock : public Noncopyable {
 public:
  void LockRead() {
    std::unique_lock<std::mutex> ulk(mutex_);
    cond_read_.wait(ulk, [=]() -> bool { return write_cnt_ == 0; });
    ++read_cnt_;
  }

  void LockWrite() {
    std::unique_lock<std::mutex> ulk(mutex_);
    ++write_cnt_;
    cond_write_.wait(ulk, [=]() -> bool { return read_cnt_ == 0 && !is_wrtiting_; });
    is_wrtiting_ = true;
  }

  void UnlockRead() {
    std::unique_lock<std::mutex> ulk(mutex_);
    if (--read_cnt_ == 0 && write_cnt_ > 0) {
      cond_write_.notify_one();
    }
  }

  void UnlockWrite() {
    std::unique_lock<std::mutex> ulk(mutex_);
    if (--write_cnt_ == 0) {
      cond_read_.notify_all();
    } else {
      cond_write_.notify_one();
    }
    is_wrtiting_ = false;
  }

 private:
  volatile size_t read_cnt_{0};
  volatile size_t write_cnt_{0};
  volatile bool is_wrtiting_{false};

  std::mutex mutex_;
  std::condition_variable cond_write_;
  std::condition_variable cond_read_;
};

class WriteLockGuard : public Noncopyable {
 public:
  explicit WriteLockGuard(RWLock &rw_lock) : rw_lock_(rw_lock) {
    rw_lock_.LockWrite();
  }

  ~WriteLockGuard() {
    rw_lock_.UnlockWrite();
  }
 private:
  RWLock &rw_lock_;
};

class ReadLockGuard : public Noncopyable {
 public:
  explicit ReadLockGuard(RWLock &rw_lock) : rw_lock_(rw_lock) {
    rw_lock_.LockRead();
  }
  ~ReadLockGuard() {
    rw_lock_.UnlockRead();
  }
 private:
  RWLock &rw_lock_;
};

}