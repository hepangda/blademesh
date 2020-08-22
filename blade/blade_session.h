//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include <utility>
#include <unordered_map>

#include "blmesh/common.h"
#include "blade/session_policy.h"
#include "blade/buffer_adaptor.h"
#include "blade/blade_object.h"

namespace blmesh {

class BladeSession : public std::enable_shared_from_this<BladeSession> {
 public:  
  BladeSession(nets::IoContext &io_context, nets::TcpSocket &&socket, Ptr<SessionPolicy> policy);
  BladeSession(nets::IoContext &io_context, nets::TcpEndpoint endpoint, Ptr<SessionPolicy> policy);

  void Start();

  void Hold(const std::string &str, Rc<BladeSession> o);
  nets::IoContext &GetIoContext();

  void ExecWrite();
  void ExecRead();
  void ExecConnect();

  void SetBladeObject(Ptr<BladeObject> obj);

  Rc<BladeSession> GetSession(const std::string &name);

  Ptr<BladeObject> &GetObject();

  void ExecChildDie(Rc<BladeSession> &child);

 private:
  void DoConnect();
  void DoWrite();
  void DoReadHeader();
  void DoReadBody();
  void SetBladeHeader();
  void SetBladeObject();

  nets::IoContext &io_context_;
  nets::TcpSocket socket_;
  nets::TcpEndpoint endpoint_;

  nets::BufferAdaptor send_buf_;
  nets::BufferAdaptor recv_buf_;

  Ptr<RawBladeHeader> now_header_;
  Ptr<BladeObject> now_obj_;

  Ptr<SessionPolicy> policy_;

  std::unordered_map<std::string, Rc<BladeSession>> hold_;
};

}  // namespace bf