//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include "blade/service_session_policy.h"

#include <utility>

#include "blade/blade_object.h"
#include "blade/blade_session.h"
#include "blade/sync_session.h"

namespace blmesh {

ServiceSessionPolicy::ServiceSessionPolicy(Port port) : port_(port) {}

void ServiceSessionPolicy::OnStart(Rc<BladeSession> self) { self->ExecRead(); }

void ServiceSessionPolicy::OnRecvWholeBladeObject(Rc<BladeSession> self, Ptr<BladeObject> &obj) {
  if (obj->GetType() == kBTKeepAlive) {
    LogInfo("BLADE", "REP_KA") << std::endl;

    self->SetBladeObject(obj->Clone());
  } else if (obj == nullptr || obj->GetFieldType(kRequestService) != FieldType::String) {
    LogError("BLADE", "WRONG_FMT") << std::endl;

    if (null_object_error_handler_) {
      null_object_error_handler_();
    }
  } else if (f_.find(obj->GetString(kRequestService)) == f_.end()) {
    LogError("BLADE", "NSS") << LogItem("Attempted", obj->GetString(kRequestService)) << std::endl;

    if (nss_error_handler_) {
      nss_error_handler_(obj->GetServiceName());
    }
  } else {
    LogInfo("BLADE", "SRV") << LogItem("Service", obj->GetString(kRequestService)) << std::endl;

    auto result = f_[obj->GetString(kRequestService)](obj);
    self->SetBladeObject(std::move(result));
  }

  self->ExecWrite();
}

void ServiceSessionPolicy::OnErrorWhenWriting(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) {
  if (write_error_handler_) {
    write_error_handler_(ec, bytes);
  }
}

void ServiceSessionPolicy::OnErrorWhenReadingHeader(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) {
  if (read_error_handler_) {
    read_error_handler_(ec, bytes);
  }
}

void ServiceSessionPolicy::OnErrorWhenReadingBody(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) {
  if (read_error_handler_) {
    read_error_handler_(ec, bytes);
  }
}

void ServiceSessionPolicy::Delegate(const std::string &service_name, ServiceHandler handler) {
  DelegateExt(service_name, handler);

  auto obj = BladeObject::NewRegister(service_name, "", port_);
  SyncSession::Call(std::move(obj));
}

void ServiceSessionPolicy::DelegateExt(const std::string &service_name, ServiceHandler handler) {
  f_.emplace(service_name, handler);
}

void ServiceSessionPolicy::SetReadErrorHandler(IoErrorHandler handler) { read_error_handler_ = handler; }

void ServiceSessionPolicy::SetWriteErrorHandler(IoErrorHandler handler) { write_error_handler_ = handler; }

void ServiceSessionPolicy::SetNssErrorHandler(NoSuchServiceErrorHandler handler) { nss_error_handler_ = handler; }

void ServiceSessionPolicy::SetNullObjectErrorHandler(NullObjectErrorHandler handler) {
  null_object_error_handler_ = handler;
}

Ptr<SessionPolicy> ServiceSessionPolicy::Clone() {
  auto new_ptr = std::make_unique<ServiceSessionPolicy>(port_);
  new_ptr->f_ = f_;
  return new_ptr;
}

}  // namespace blmesh