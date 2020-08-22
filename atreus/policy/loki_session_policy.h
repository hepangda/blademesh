#pragma once

#include "blade/session_policy.h"
#include "blmesh/common.h"

namespace blmesh {

class BladeSession;
class BladeObject;

class LokiSessionPolicy : public SessionPolicy {
 public:
  void OnStart(Rc<BladeSession> self) override;
  void OnErrorWhenReadingBody(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) override;
  void OnErrorWhenReadingHeader(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) override;
  void OnRecvWholeBladeObject(Rc<BladeSession> self, Ptr<BladeObject> &obj) override;
  void OnChildDie(Rc<BladeSession> self, Rc<BladeSession> child) override;

  Ptr<SessionPolicy> Clone() override { return std::make_unique<LokiSessionPolicy>(); }

  static Ptr<SessionPolicy> Use() { return std::make_unique<LokiSessionPolicy>(); }
};

}  // namespace blmesh
