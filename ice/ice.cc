//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include <iostream>

#include "blade/blade_object.h"
#include "blade/blade_server.h"
#include "blade/service_session_policy.h"
#include "ice/service_manager.h"

using namespace blmesh;

int main(int argc, char *argv[]) {
  auto policy = std::make_unique<ServiceSessionPolicy>(kIcePort);
  policy->DelegateExt(kIceServiceName, [] Service(obj) {
    Ptr<BladeObject> ret;

    try {
      auto name = obj->GetString(kQuery);
      auto res = ServiceManager::Get().Query(name);

      if (res == nullptr) {
        throw std::exception{};
      }
      ret = BladeObject::NewAnswer(kIceServiceName, obj->GetTraceId(), name, *res);

      LogInfo("Ice", "QUERY") << LogItem("Service", name) << LogItem("IP", *res) << std::endl;

    } catch (...) {
      ret = BladeObject::NewAnswer(kIceServiceName);

      LogInfo("Ice", "QUERY") << LogItem("Service", obj->GetString(kQuery)) << LogItem("IP", "#NO_ANSWER#")
                              << std::endl;
    }

    return ret;
  });

  policy->DelegateExt(kIceRegisterServiceName, [] Service(obj) {
    Ptr<BladeObject> ret =
        BladeObject::NewResponse(kIceRegisterServiceName, obj->GetTraceId(), obj->GetString(kRequestService));

    try {
      auto ip = obj->GetString(kRegisterIp);
      auto service_name = obj->GetServiceName();

      if (obj->GetType() == kBTRegister) {
        ServiceManager::Get().Register(service_name, ip);
        obj->AddByte(kIceOk, kZeroByte);

        LogInfo("Ice", "REGISTER") << LogItem("Service", service_name) << LogItem("IP", ip) << std::endl;

      } else if (obj->GetType() == kBTDeregister) {
        ServiceManager::Get().Deregister(service_name, ip);
        obj->AddByte(kIceOk, kZeroByte);

        LogInfo("Ice", "DEREGISTER") << LogItem("Service", service_name) << LogItem("IP", ip) << std::endl;
      } else {
        throw std::invalid_argument("unreachable code");
      }
    } catch (...) {
      obj->AddByte(kIceFailed, kZeroByte);
      LogException("Ice") << std::endl;
    }

    return ret;
  });

  policy->DelegateExt(kChaosAvaSrv, [] Service(obj) {
    Ptr<BladeObject> ret = BladeObject::NewResponse(kChaosAvaSrv, obj->GetTraceId(), obj->GetString(kRequestService));

    try {
      auto res = ServiceManager::Get().GetSrv();
      for (auto &i : res) {
        ret->AddByte(i, static_cast<BladeByte>(1));
      }
      ret->AddByte(kChaosOk, kZeroByte);
    } catch (...) {
    }

    return ret;
  });

  policy->DelegateExt(kChaosAvaSrvN, [] Service(obj) {
    Ptr<BladeObject> ret = BladeObject::NewResponse(kChaosAvaSrvN, obj->GetTraceId(), obj->GetString(kRequestService));

    try {
      int res = ServiceManager::Get().GetSrvN();
      ret->AddI32(kChaosVal, res);
      ret->AddByte(kChaosOk, kZeroByte);
    } catch (...) {
    }

    return ret;
  });

  auto blade_server = std::make_shared<BladeServer>(kIcePort, std::move(policy));
  blade_server->Start();

  return 0;
}