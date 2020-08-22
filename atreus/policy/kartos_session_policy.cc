#include "kartos_session_policy.h"

#include "atreus/util/config_provider.h"
#include "atreus/util/data_manager.h"
#include "atreus/util/service_manager.h"
#include "blade/blade_object.h"
#include "blade/blade_session.h"
#include "blade/client_session_policy.h"

namespace blmesh {

void KartosSessionPolicy::OnErrorWhenReadingBody(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) {
  LogError("Kartos", "IOERR") << LogItem("Pos", "body") << LogItem("Error", ec.message()) << std::endl;
}

void KartosSessionPolicy::OnErrorWhenReadingHeader(Rc<BladeSession> self, const nets::ErrorCode &ec, size_t bytes) {
  LogError("Kartos", "IOERR") << LogItem("Pos", "header") << LogItem("Error", ec.message()) << std::endl;
}

void KartosSessionPolicy::OnRecvWholeBladeObject(Rc<BladeSession> self, Ptr<BladeObject> &obj) {
  try {
    if (obj->GetType() == kBTRequest) {
      auto qsrv_name = obj->GetString(kRequestService);
      LogInfo("Kartos", "REQ") << LogItem("Caller", obj->GetServiceName()) << LogItem("Callee", qsrv_name) << std::endl;

      DataManager::Get().DoQuery(qsrv_name);
      DataManager::Get().DoBytes(qsrv_name, obj->GetPayloadLength());

      auto port = ServiceManager::Get().Query(qsrv_name);
      if (port == nullptr) {
        throw std::invalid_argument{"no addr"};
      }

      auto nes = std::make_shared<BladeSession>(self->GetIoContext(),
                                                nets::MakeTcpEndpoint(*port, ConfigProvider::GetThisIp()),
                                                ClientSessionPolicy::Use("kartos"));
      nes->Hold("kartos", self);
      nes->Start();
    }
  } catch (NoSuchFieldException &ex) {
    LogException("Kartos") << LogItem("Message", "No Such Field") << std::endl;
  } catch (std::invalid_argument &ex) {
    LogException("Kartos") << LogItem("Message", ex.what()) << std::endl;
  }
}

void KartosSessionPolicy::OnStart(Rc<BladeSession> self) { self->ExecRead(); }

void KartosSessionPolicy::OnChildDie(Rc<BladeSession> self, Rc<BladeSession> child) { self->ExecWrite(); }

}  // namespace blmesh