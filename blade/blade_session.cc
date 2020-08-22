//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include "blade_session.h"

#include <utility>

#include "blade/opr.h"

namespace blmesh {

BladeSession::BladeSession(nets::IoContext &io_context, nets::TcpSocket &&socket, Ptr<SessionPolicy> policy)
    : io_context_(io_context), socket_(std::move(socket)), policy_(std::move(policy)) {}

BladeSession::BladeSession(nets::IoContext &io_context, nets::TcpEndpoint endpoint, Ptr<SessionPolicy> policy)
    : io_context_(io_context), socket_(io_context), endpoint_(std::move(endpoint)), policy_(std::move(policy)) {}

void BladeSession::Start() {
  auto self = shared_from_this();
  self->policy_->OnStart(self);
}

void BladeSession::Hold(const std::string &str, Rc<BladeSession> o) { hold_.emplace(str, std::move(o)); }

nets::IoContext &BladeSession::GetIoContext() { return io_context_; }

void BladeSession::ExecWrite() { DoWrite(); }

void BladeSession::ExecRead() { DoReadHeader(); }

void BladeSession::ExecConnect() { DoConnect(); }

void BladeSession::SetBladeObject(Ptr<BladeObject> obj) { now_obj_ = std::move(obj); }

Rc<BladeSession> BladeSession::GetSession(const std::string &name) {
  if (hold_.find(name) == hold_.end()) {
    return nullptr;
  }
  return hold_[name];
}

Ptr<BladeObject> &BladeSession::GetObject() { return now_obj_; }

void BladeSession::ExecChildDie(Rc<BladeSession> &child) {
  auto self = shared_from_this();
  policy_->OnChildDie(self, child);
}

void BladeSession::DoConnect() {
  auto self = shared_from_this();
  socket_.async_connect(self->endpoint_, [self] NetConnectHandler(ec) {
    if (ec) {
      self->policy_->OnErrorWhenConnect(self, ec);
    } else {
      self->policy_->OnConnected(self);
    }
  });
}

void BladeSession::DoWrite() {
  auto self = shared_from_this();

  auto size_required = now_obj_->GetEncodedSize();
  auto buffer = send_buf_.PrepareBuffer(size_required);
  blmesh::Opr::Encode(*now_obj_, buffer.data(), size_required);
  send_buf_.Produce(size_required);

  net::async_write(socket_, send_buf_.GetBuffer(), [self, size_required] NetWriteHandler(ec, bytes) {
    self->send_buf_.Consume(size_required);

    if (ec || bytes != size_required) {
      return self->policy_->OnErrorWhenWriting(self, ec, bytes);
    }

    self->now_obj_.reset();
    self->policy_->OnWrote(self);
  });
}

void BladeSession::DoReadHeader() {
  auto self = shared_from_this();
  static constexpr auto header_size = sizeof(RawBladeHeader);
  net::async_read(socket_, recv_buf_.PrepareBuffer(header_size), [self] NetReadHandler(ec, bytes) {
    if (ec || bytes != header_size) {
      return self->policy_->OnErrorWhenReadingHeader(self, ec, bytes);
    }

    {
      auto g = self->recv_buf_.MakeGuard(header_size);
      self->SetBladeHeader();
    }

    self->DoReadBody();
  });
}

void BladeSession::DoReadBody() {
  auto self = shared_from_this();
  auto body_length = now_header_->payload_length;

  net::async_read(socket_, recv_buf_.PrepareBuffer(body_length), [self, body_length] NetReadHandler(ec, bytes) {
    if (ec || bytes != body_length) {
      return self->policy_->OnErrorWhenReadingBody(self, ec, bytes);
    }

    {
      auto g = self->recv_buf_.MakeGuard(body_length);
      self->SetBladeObject();
    }

    self->policy_->OnRecvWholeBladeObject(self, self->now_obj_);
  });
}

void BladeSession::SetBladeHeader() {
  now_obj_ = nullptr;
  if (recv_buf_.GetSize() < sizeof(RawBladeHeader)) {
    return;
  }

  auto header = Opr::GetRawBladeHeaderFromPtr(recv_buf_.GetBuffer().data());
  recv_buf_.Consume(sizeof(RawBladeHeader));
  now_header_ = std::make_unique<RawBladeHeader>(header);
}

void BladeSession::SetBladeObject() {
  now_obj_ = blmesh::Opr::DecodeWithHeader(*now_header_, recv_buf_.GetBuffer().data(), recv_buf_.GetSize());
}

}  // namespace blmesh