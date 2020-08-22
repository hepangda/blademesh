#include "loki.h"

#include "atreus/policy/loki_session_policy.h"
#include "blmesh/common.h"

namespace blmesh {

Loki::Loki() : server_(kLokiPort, LokiSessionPolicy::Use()) {}

void Loki::Start() { server_.Start(); }

}  // namespace blmesh