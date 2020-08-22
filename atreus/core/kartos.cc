#include "kartos.h"

#include "atreus/policy/kartos_session_policy.h"
#include "blmesh/common.h"

namespace blmesh {

Kartos::Kartos() : server_(kKartosPort, KartosSessionPolicy::Use()) {}

void Kartos::Start() { server_.Start(); }

}  // namespace blmesh
