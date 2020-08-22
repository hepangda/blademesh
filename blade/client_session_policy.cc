//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include "client_session_policy.h"

#include <utility>

#include "blade/blade_session.h"
#include "blade/blade_object.h"

namespace blmesh {

void ClientSessionPolicy::OnStart(Rc<BladeSession> self) { self->ExecConnect(); }

void ClientSessionPolicy::OnConnected(Rc<BladeSession> self) {
  auto father_self = self->GetSession(father_name_);
  auto &father_obj = father_self->GetObject();
  auto send_obj = father_obj->Clone();

  self->SetBladeObject(std::move(send_obj));

  self->ExecWrite();
}

void ClientSessionPolicy::OnWrote(Rc<BladeSession> self) { self->ExecRead(); }

void ClientSessionPolicy::OnRecvWholeBladeObject(Rc<BladeSession> self, Ptr<BladeObject> &obj) {
  auto father_self = self->GetSession(father_name_);
  auto resp_obj = obj->Clone();

  father_self->SetBladeObject(std::move(resp_obj));
  father_self->ExecChildDie(self);
}

Ptr<SessionPolicy> ClientSessionPolicy::Clone() {
  auto ret = std::make_unique<ClientSessionPolicy>();
  ret->father_name_ = father_name_;
  return ret;
}

Ptr<SessionPolicy> ClientSessionPolicy::Use(std::string father_name) {
  auto ret = std::make_unique<ClientSessionPolicy>();
  ret->father_name_ = std::move(father_name);
  return ret;
}

}  // namespace blmesh