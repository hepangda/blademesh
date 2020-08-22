//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include <string>

#include "blmesh/common.h"
#include "blade/session_policy.h"

namespace blmesh {

class ClientSessionPolicy : public SessionPolicy {
 public:
  void OnStart(Rc<BladeSession> self) override;
  void OnConnected(Rc<BladeSession> self) override;
  void OnWrote(Rc<BladeSession> self) override;
  void OnRecvWholeBladeObject(Rc<BladeSession> self, Ptr<BladeObject> &obj) override;
  
  Ptr<SessionPolicy> Clone() override;

  static Ptr<SessionPolicy> Use(std::string father_name);
 private:
  std::string father_name_;
};

}