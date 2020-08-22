//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include "blmesh/common.h"

namespace blmesh::nets {

class BufferAdaptor {
 public:
  struct BufferGuard {
   public:
    BufferGuard(BufferAdaptor &adaptor, size_t size) : size_(size), adaptor_(adaptor) { adaptor_.Produce(size_); }

    ~BufferGuard() { adaptor_.Consume(size_); }

   private:
    size_t size_;
    BufferAdaptor &adaptor_;
  };

  [[nodiscard]] size_t GetSize() const { return buf_.size(); }

  [[nodiscard]] size_t GetCapacity() const { return buf_.capacity(); }

  [[nodiscard]] nets::MutableBuffer PrepareBuffer(size_t size) { return buf_.prepare(size); }

  void Produce(size_t size) { return buf_.commit(size); }

  BufferGuard MakeGuard(size_t size) { return BufferGuard{*this, size}; }

  void Consume(size_t size) { return buf_.consume(size); }

  bool Consume(void *ptr, size_t size) {
    auto buf = GetBuffer();
    if (buf.size() < size) {
      return false;
    }

    memcpy(ptr, GetBuffer().data(), size);
    Consume(size);
    return true;
  }

  [[nodiscard]] nets::ConstBuffer GetBuffer() const { return buf_.data(); }

 private:
  nets::StreamBuf buf_;
};

}  // namespace blmesh::nets
