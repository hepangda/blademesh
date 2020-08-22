//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include <cstdlib>

namespace blmesh {

class RemainLengthNotEnoughException : public std::exception {};

class Decoder {
 public:
  explicit Decoder(byte *origin, int raw_length)
      : cur_(origin), origin_(origin), raw_length_(raw_length), remain_length_(raw_length) {}

  template <typename T>
  T Take() {
    auto g = CurRemainGuard{sizeof(T), cur_, remain_length_};

    T ret;
    memcpy(&ret, cur_, sizeof(T));
    return ret;
  }

  BladeString TakeBladeString() {
    u32 length = Take<u32>();
    auto g = CurRemainGuard{length, cur_, remain_length_};

    return {reinterpret_cast<char *>(cur_), length};
  }

  inline BladeByte TakeBladeByte() { return Take<BladeByte>(); }

  inline BladeI32 TakeBladeI32() { return Take<BladeI32>(); }

  inline BladeI64 TakeBladeI64() { return Take<BladeI64>(); }

  inline BladeF64 TakeBladeF64() { return Take<BladeF64>(); }

  [[nodiscard]] bool MinSizeSatisfied(int size) const {
    size -= sizeof(RawBladeHeader) + sizeof(u32);
    if (size < 0) {
      return false;
    }

    u32 srv_name_length = *reinterpret_cast<u32 *>(origin_ + sizeof(RawBladeHeader));
    return size >= srv_name_length;
  }

  [[nodiscard]] bool NotEnd() const { return remain_length_ > 0; }

 private:
  struct CurRemainGuard {
   public:
    CurRemainGuard(size_t need, byte *&cur, int &remain_length) : need_(need), cur_(cur) {
      if (remain_length >= need) {
        remain_length -= need;
      } else {
        throw RemainLengthNotEnoughException{};
      }
    }

    ~CurRemainGuard() { cur_ += need_; }

   private:
    size_t need_;
    byte *&cur_;
  };

  void TakePtr(void *dest, int length) {
    memcpy(dest, cur_, length);
    cur_ += length;
  }

  byte *origin_;
  byte *cur_;
  int raw_length_;
  int remain_length_;
};

}  // namespace blmesh