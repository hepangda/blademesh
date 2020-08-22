//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include "sync_session.h"

#include <utility>

#include "blade/blade_object.h"
#include "blade/opr.h"

namespace blmesh {

SyncSession::SyncSession(nets::TcpEndpoint endpoint) : endpoint_(std::move(endpoint)) {}

Ptr<BladeObject> SyncSession::Call(Ptr<BladeObject> obj) {
  SyncSession session{nets::MakeTcpEndpoint(kLokiPort, "127.0.0.1")};
  session.SetObject(std::move(obj));
  session.Start();
  return session.GetObject()->Clone();
}

Ptr<BladeObject> SyncSession::CallNotSure(Ptr<BladeObject> obj) {
  SyncSession session{nets::MakeTcpEndpoint(kLokiPort, "127.0.0.1")};
  session.SetObject(std::move(obj));
  if (!session.Start()) {
    return nullptr;
  }
  return session.GetObject()->Clone();
}

Ptr<BladeObject> SyncSession::CallExt(Ptr<BladeObject> obj, Port port, const std::string &ip) {
  SyncSession session{nets::MakeTcpEndpoint(port, ip)};
  session.SetObject(std::move(obj));
  session.Start();
  return session.GetObject()->Clone();
}

Ptr<BladeObject> SyncSession::CallExtNotSure(Ptr<BladeObject> obj, Port port, const std::string &ip) {
  SyncSession session{nets::MakeTcpEndpoint(port, ip)};
  session.SetObject(std::move(obj));
  auto ec = session.Start();
  if (ec) {
    return nullptr;
  }
  return session.GetObject()->Clone();
}

nets::ErrorCode SyncSession::Start() {
  nets::ErrorCode ec;
  socket_.connect(endpoint_, ec);
#define check_and_return_ec \
  if (ec) return ec
  check_and_return_ec;

  {
    auto size_required = now_obj_->GetEncodedSize();
    auto buffer = send_buf_.PrepareBuffer(size_required);
    Opr::Encode(*now_obj_, buffer.data(), size_required);

    auto g = send_buf_.MakeGuard(size_required);

    net::write(socket_, send_buf_.GetBuffer(), ec);
    check_and_return_ec;
  }
  now_obj_.reset();

  static constexpr auto header_size = sizeof(RawBladeHeader);
  net::read(socket_, recv_buf_.PrepareBuffer(header_size), ec);
  check_and_return_ec;

  {
    auto g = recv_buf_.MakeGuard(header_size);
    SetBladeHeader();
  }

  auto body_length = now_header_->payload_length;

  net::read(socket_, recv_buf_.PrepareBuffer(body_length), ec);
  check_and_return_ec;

  {
    auto g = recv_buf_.MakeGuard(body_length);
    SetBladeObject();
  }
#undef check_and_return_ec
  return ec;
}

void SyncSession::SetObject(Ptr<BladeObject> obj) { now_obj_ = std::move(obj); }

Ptr<BladeObject> &SyncSession::GetObject() { return now_obj_; }

void SyncSession::SetBladeHeader() {
  now_obj_ = nullptr;
  if (recv_buf_.GetSize() < sizeof(RawBladeHeader)) {
    return;
  }

  auto header = Opr::GetRawBladeHeaderFromPtr(recv_buf_.GetBuffer().data());
  recv_buf_.Consume(sizeof(RawBladeHeader));
  now_header_ = std::make_unique<RawBladeHeader>(header);
}

void SyncSession::SetBladeObject() {
  now_obj_ = blmesh::Opr::DecodeWithHeader(*now_header_, recv_buf_.GetBuffer().data(), recv_buf_.GetSize());
}

}  // namespace blmesh
