//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include "blade_server.h"

#include "blade/blade_session.h"

namespace blmesh {

BladeServer::BladeServer(Port port, Ptr<SessionPolicy> policy)
    : io_context_(1), acceptor_(io_context_, nets::MakeTcpEndpoint(port)), policy_(std::move(policy)) {
  DoAccept();
}

BladeServer::BladeServer() : io_context_(1), acceptor_(io_context_), policy_(nullptr) {}

void BladeServer::Start() {
  Log::Init("");
  FLAGS_logtostderr = true;
  io_context_.run();
}

void BladeServer::SetPolicy(Ptr<SessionPolicy> policy) { policy_ = std::move(policy); }

void BladeServer::DoAccept() {
  acceptor_.async_accept([this] NetMoveAcceptHandler(ec, socket) {
    auto session = std::make_shared<BladeSession>(io_context_, std::move(socket), policy_->Clone());
    session->Start();
    DoAccept();
  });
}

}  // namespace blmesh