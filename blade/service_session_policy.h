//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include <string>
#include <functional>
#include <unordered_map>

#include "blmesh/common.h"
#include "blade/session_policy.h"

namespace blmesh {

class BladeSession;
class BladeObject;

class ServiceSessionPolicy : public SessionPolicy {
 public:
  explicit ServiceSessionPolicy(Port port);

  using NoSuchServiceErrorHandler = std::function<void(const std::string &)>;
  using NullObjectErrorHandler = std::function<void()>;
  using ServiceHandler = std::function<Ptr<BladeObject>(Ptr<BladeObject> &)>;
  using IoErrorHandler = std::function<void(const nets::ErrorCode &ec, size_t bytes)>;

  void OnStart(Rc<BladeSession> self) override;
  void OnRecvWholeBladeObject(Rc<BladeSession> self, Ptr<BladeObject> &obj) override;
  void OnErrorWhenWriting(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) override;
  void OnErrorWhenReadingHeader(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) override;
  void OnErrorWhenReadingBody(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) override;


  void Delegate(const std::string &service_name, ServiceHandler handler);
  void DelegateExt(const std::string &service_name, ServiceHandler handler);

  void SetReadErrorHandler(IoErrorHandler handler);
  void SetWriteErrorHandler(IoErrorHandler handler);
  void SetNssErrorHandler(NoSuchServiceErrorHandler handler);
  void SetNullObjectErrorHandler(NullObjectErrorHandler handler);

  ~ServiceSessionPolicy() override = default;

  Ptr<SessionPolicy> Clone() override;
 private:
  std::unordered_map<std::string, ServiceHandler> f_;
  Port port_;
  IoErrorHandler read_error_handler_;
  IoErrorHandler write_error_handler_;
  NoSuchServiceErrorHandler nss_error_handler_;
  NullObjectErrorHandler null_object_error_handler_;
};

}