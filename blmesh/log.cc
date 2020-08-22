//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include "log.h"

#include <chrono>
#include <iostream>

#include <glog/logging.h>

namespace blmesh {

void Log::Init(const char *argv0) {
  static bool init = true;
  if (init) {
    google::InitGoogleLogging(argv0);
    FLAGS_log_dir = "./logs";
    FLAGS_alsologtostderr = true;
    google::InstallFailureSignalHandler();

    google::SetLogDestination(google::GLOG_INFO, (Get().GetEpoch() + "_info").c_str());
    google::SetLogDestination(google::GLOG_ERROR, (Get().GetEpoch() + "_error").c_str());
    init = false;
  }
}

void Log::SetPrefix(const std::string &prefix) { Get().SetIPrefix(prefix); }

void Log::Shutdown() { return google::ShutdownGoogleLogging(); }

void Log::UpdateDest() {
  auto prefix = Get().GeneratePrefix();
  google::SetLogDestination(google::GLOG_INFO, (prefix + "_info").c_str());
  google::SetLogDestination(google::GLOG_ERROR, (prefix + "_error").c_str());
}

Log &Log::Get() {
  static Log instance;
  return instance;
}

std::string Log::GetLastEpoch() { return last_epoch_; }

std::string Log::GetEpoch() { return now_epoch_; }

std::string Log::Update() {
  last_epoch_ = now_epoch_;
  return now_epoch_ = GenerateEpoch();
}

Log::Log() : last_epoch_(GenerateEpoch()), now_epoch_(GenerateEpoch()) {}

std::string Log::GenerateEpoch() {
  auto sc = std::chrono::system_clock::now().time_since_epoch();
  auto timepoint = std::chrono::duration_cast<std::chrono::seconds>(sc).count();

  return std::to_string(timepoint);
}

void Log::SetIPrefix(const std::string &prefix) { iprefix_ = prefix; }

std::string Log::GeneratePrefix() { return iprefix_ + Update(); }

}  // namespace blmesh