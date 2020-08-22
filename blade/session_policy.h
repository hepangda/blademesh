//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.
  
#pragma once

#include "blmesh/common.h"

namespace bf {

class BladeObject;
class BladeSession;

class SessionPolicy {
 public:
  virtual void OnStart(Rc<BladeSession> self) {}
  virtual void OnRecvWholeBladeObject(Rc<BladeSession> self, Ptr<BladeObject> &obj) {}
  virtual void OnConnected(Rc<BladeSession> self) {}
  virtual void OnWrote(Rc<BladeSession> self) {}
  virtual void OnChildDie(Rc<BladeSession> self, Rc<BladeSession> child) {}


  virtual void OnErrorWhenConnect(Rc<BladeSession> self, const nets::ErrorCode &ec) {}
  virtual void OnErrorWhenWriting(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) {}

  virtual void OnErrorWhenReadingHeader(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) {}
  virtual void OnErrorWhenReadingBody(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) {}

  virtual ~SessionPolicy() = default;
  virtual Ptr<SessionPolicy> Clone() = 0;
};

}