//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include "blmesh/common.h"

namespace blmesh {
    
class BladeObject;

namespace Opr {

template <typename T>
T GetObjectFromPtr(void *ptr) {
  return *reinterpret_cast<T *>(ptr);
}

RawBladeHeader GetRawBladeHeaderFromPtr(const void *ptr);
int Encode(const BladeObject &object, void *dest, int length_limit);

Ptr<BladeObject> DecodeWithHeader(RawBladeHeader header, void *origin_v, int raw_length);
Ptr<BladeObject> DecodeWithHeader(RawBladeHeader header, const void *origin_v, int raw_length);

Ptr<BladeObject> Decode(void *origin_v, int raw_length);
Ptr<BladeObject> Decode(const void *origin_v, int raw_length);

}  // namespace Opr
}  // namespace blmesh