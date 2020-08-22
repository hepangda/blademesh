#include "watchdog.h"

#include "atreus/util/config_provider.h"
#include "atreus/util/data_manager.h"
#include "atreus/util/service_manager.h"
#include "blade/blade_object.h"
#include "blade/sync_session.h"

namespace blmesh {

void Watchdog::Start() {
  server_.ScheduledTask(ConfigProvider::GetKeepAliveDuration(), [this] NetTimerHandler(ec) {
    if (ec) {
      LogError("WatchDog", "IOERR") << LogItem("Pos", "keep_alive_timer") << LogItem("Error", ec.message())
                                    << std::endl;
    }
    Check();
  });

  server_.ScheduledTask(ConfigProvider::GetReportDurtaion(), [this] NetTimerHandler(ec) {
    if (ec) {
      LogError("WatchDog", "IOERR") << LogItem("Pos", "report_timer") << LogItem("Error", ec.message()) << std::endl;
    }
    Report();
  });

  server_.Start();
}

void Watchdog::Check() {
  LogInfo("WatchDog", "CHECK") << LogItem("ServNum", ServiceManager::Get().GetSize()) << std::endl;

  std::vector<std::pair<std::string, Port>> need_deregister;
  auto g = WriteLockGuard{ServiceManager::Get().GetLock_UNSAFE()};
  ServiceManager::Get().Iterate_UNSAFE([&need_deregister](const std::string &name, Port port) {
    auto message = BladeObject::NewKeepAlive(kKartosServiceName);
    auto result = SyncSession::CallExtNotSure(std::move(message), port, ConfigProvider::GetThisIp());

    if (result == nullptr) {
      need_deregister.emplace_back(name, port);
    }
  });

  for (auto &i : need_deregister) {
    ServiceManager::Get().Deregister_UNSAFE(i.first);

    LogInfo("WatchDog", "DERLOCAL") << LogItem("Service", i.first) << std::endl;
    {
      auto sd_message = BladeObject::NewDeregister(i.first, ConfigProvider::GetThisIp());
      auto ret = SyncSession::CallExt(std::move(sd_message), kIcePort, ConfigProvider::GetIceIp());

      LogInfo("WatchDog", "DEREMOTE") << LogItem("Service", i.first) << std::endl;
      DataManager::Get().DoShutdown(i.first);
    }
  }
}

void Watchdog::Report() {
  LogInfo("WatchDog", "REPORT") << LogItem("Epoch", Log::Get().GetEpoch()) << std::endl;

  {
    auto rep = DataManager::Get().QueryReport();
    SyncSession::CallExt(std::move(rep), kChaosPort, ConfigProvider::GetChaosIp());
  }

  {
    auto rep = DataManager::Get().ErrorReport();
    SyncSession::CallExt(std::move(rep), kChaosPort, ConfigProvider::GetChaosIp());
  }

  {
    auto rep = DataManager::Get().ShutdownReport();
    SyncSession::CallExt(std::move(rep), kChaosPort, ConfigProvider::GetChaosIp());
  }

  {
    auto rep = DataManager::Get().BytesReport();
    SyncSession::CallExt(std::move(rep), kChaosPort, ConfigProvider::GetChaosIp());
  }
}

}  // namespace blmesh