//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include <utility>

#include "blmesh/common.h"
#include "blade/buffer_adaptor.h"

namespace blmesh {

class BladeObject;

class SyncSession {
 public:
  explicit SyncSession(nets::TcpEndpoint endpoint);

  static Ptr<BladeObject> Call(Ptr<BladeObject> obj);
  static Ptr<BladeObject> CallNotSure(Ptr<BladeObject> obj);
  static Ptr<BladeObject> CallExt(Ptr<BladeObject> obj, Port port, const std::string &ip);
  static Ptr<BladeObject> CallExtNotSure(Ptr<BladeObject> obj, Port port, const std::string &ip);

  nets::ErrorCode Start();

  void SetObject(Ptr<BladeObject> obj);
  Ptr<BladeObject> &GetObject();

  void SetBladeHeader();
  void SetBladeObject();
 private:
  nets::IoContext io_context_{1};
  nets::TcpSocket socket_{io_context_};
  nets::TcpEndpoint endpoint_;

  nets::BufferAdaptor send_buf_;
  nets::BufferAdaptor recv_buf_;

  Ptr<RawBladeHeader> now_header_;
  Ptr<BladeObject> now_obj_;
};

}
