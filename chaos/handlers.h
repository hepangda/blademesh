//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include "blade/blade_object.h"
#include "blmesh/common.h"

namespace blmesh {

Ptr<BladeObject> DoReport(Ptr<BladeObject> &obj);
Ptr<BladeObject> DoQuery(Ptr<BladeObject> &obj);
Ptr<BladeObject> DoError(Ptr<BladeObject> &obj);
Ptr<BladeObject> DoShutdown(Ptr<BladeObject> &obj);
Ptr<BladeObject> DoBytes(Ptr<BladeObject> &obj);
Ptr<BladeObject> DoMostQuery(Ptr<BladeObject> &obj);
Ptr<BladeObject> DoMostError(Ptr<BladeObject> &obj);
Ptr<BladeObject> DoMostShutdown(Ptr<BladeObject> &obj);

}  // namespace blmesh