#include "loki_session_policy.h"

#include "atreus/util/config_provider.h"
#include "atreus/util/data_manager.h"
#include "atreus/util/service_manager.h"
#include "blade/blade_object.h"
#include "blade/blade_session.h"
#include "blade/client_session_policy.h"
#include "blade/sync_session.h"

namespace blmesh {

void LokiSessionPolicy::OnErrorWhenReadingBody(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) {
  LogError("Loki", "IOERR") << LogItem("Pos", "body") << LogItem("Error", ec.message()) << std::endl;
}

void LokiSessionPolicy::OnErrorWhenReadingHeader(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) {
  LogError("Loki", "IOERR") << LogItem("Pos", "header") << LogItem("Error", ec.message()) << std::endl;
}

void LokiSessionPolicy::OnRecvWholeBladeObject(Rc<BladeSession> self, Ptr<BladeObject> &obj) {
  try {
    if (obj->GetType() == kBTRegister) {
      auto port = static_cast<Port>(obj->GetI32(kRegisterPort));

      LogInfo("Loki", "REGISTER") << LogItem("Service", obj->GetServiceName()) << LogItem("Port", port) << std::endl;

      ServiceManager::Get().Register(obj->GetServiceName(), port);
      {
        // Register to ice
        auto sd_message = BladeObject::NewRegister(obj->GetServiceName(), ConfigProvider::GetThisIp(), port);
        auto ret = SyncSession::CallExt(std::move(sd_message), kIcePort, ConfigProvider::GetIceIp());
      }
      self->ExecWrite();
    } else if (obj->GetType() == kBTDeregister) {
      LogInfo("Loki", "DEREGISTER") << LogItem("Service", obj->GetServiceName()) << std::endl;

      ServiceManager::Get().Deregister(obj->GetServiceName());
      {
        // Deregister to ice
        auto sd_message = BladeObject::NewDeregister(obj->GetServiceName(), ConfigProvider::GetThisIp());
        auto ret = SyncSession::CallExt(std::move(sd_message), kIcePort, ConfigProvider::GetIceIp());
      }
      self->ExecWrite();
    } else if (obj->GetTraceId() == kBTRequest) {
      LogInfo("Loki", "REQ") << LogItem("Caller", obj->GetServiceName())
                             << LogItem("Callee", obj->GetString(kRequestService)) << std::endl;

      auto srv_name = obj->GetString(kRequestService);
      // auto addr = ServiceManager::Get().Query(srv_name);
      Ptr<Port> port = nullptr;
      if (port != nullptr) {
        auto nes = std::make_shared<BladeSession>(self->GetIoContext(),
                                                  nets::MakeTcpEndpoint(*port, ConfigProvider::GetThisIp()),
                                                  ClientSessionPolicy::Use("loki"));
        nes->Hold("loki", self);
        nes->Start();
      } else {
        auto query_obj = BladeObject::NewQuery(kLokiServiceName, srv_name);
        auto result = SyncSession::CallExt(std::move(query_obj), kIcePort, ConfigProvider::GetIceIp());

        if (result == nullptr || result->GetFieldType(kNoAnswer) != FieldType::None) {
          auto res =
              BladeObject::NewResponse(obj->GetString(kRequestService), obj->GetTraceId(), obj->GetServiceName());
          res->AddByte(kNoService, kZeroByte);
          DataManager::Get().DoError(obj->GetString(kRequestService));

          self->SetBladeObject(std::move(res));
          self->ExecWrite();
        } else {
          auto ip = result->GetString(kAnswerIp);
          auto nes = std::make_shared<BladeSession>(self->GetIoContext(), nets::MakeTcpEndpoint(kKartosPort, ip),
                                                    ClientSessionPolicy::Use("loki"));
          nes->Hold("loki", self);
          nes->Start();
        }
      }
    }
  } catch (...) {
    LogException("Loki") << LogItem("Message", "") << std::endl;
  }
}

void LokiSessionPolicy::OnStart(Rc<BladeSession> self) { self->ExecRead(); }

void LokiSessionPolicy::OnChildDie(Rc<BladeSession> self, Rc<BladeSession> child) { self->ExecWrite(); }

}  // namespace blmesh