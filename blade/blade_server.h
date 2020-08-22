//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include "blmesh/common.h"

namespace blmesh {

class SessionPolicy;

class BladeServer {
 public:
  BladeServer(Port port, Ptr<SessionPolicy> policy);

  BladeServer();

  void Start();

  void SetPolicy(Ptr<SessionPolicy> policy);

  template <typename Func>
  void ScheduledTask(int duration, Func f) {
    auto timer = std::make_shared<net::system_timer>(io_context_, std::chrono::seconds(duration));

    timer->async_wait([this, duration, f, timer] NetTimerHandler(ec) {
      f(ec);
      ScheduledTask(duration, f);
    });
  }

 private:
  void DoAccept();

 private:
  nets::IoContext io_context_;
  nets::TcpAcceptor acceptor_;
  Ptr<SessionPolicy> policy_;
};

}  // namespace blmesh