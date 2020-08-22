//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include <cstring>

#include "blade/blade_object.h"
#include "blmesh/common.h"

namespace blmesh {

class Encoder {
 public:
  virtual void Encode(byte *dest) = 0;
  virtual size_t GetExpectedSize() = 0;

  virtual void MoveDestPtr(byte *&ptr) { ptr += GetExpectedSize(); }

  virtual void MoveDestAndRet(byte *&ptr, int &ret) {
    ptr += GetExpectedSize();
    ret += GetExpectedSize();
  }

  [[nodiscard]] int GetAllOffset() const { return dest_offset; }

 protected:
  template <typename T>
  int Put(byte *dest, T val) {
    memcpy(dest + dest_offset, &val, sizeof(T));
    dest_offset += sizeof(T);
    return sizeof(T);
  }

  int PutPtr(byte *dest, const void *ptr, int length) {
    memcpy(dest + dest_offset, ptr, length);
    dest_offset += length;
    return length;
  }

 private:
  int dest_offset{0};
};

#define EncoderClass(ClassName, Type, SizeCalc, Statements)                          \
  class ClassName : public Encoder {                                                 \
   public:                                                                           \
    explicit ClassName(const Type &ref) : ref(ref) {}                                \
    void Encode(byte *dest) override{Statements} size_t GetExpectedSize() override { \
      return sizeof(u16) + (SizeCalc);                                               \
    }                                                                                \
                                                                                     \
   private:                                                                          \
    const Type &ref;                                                                 \
  }

EncoderClass(BladeByteEncoder, BladeByte, sizeof(BladeByte), {
  Put(dest, FieldType::Byte);
  Put(dest, ref);
});

EncoderClass(BladeI32Encoder, BladeI32, sizeof(BladeI32), {
  Put(dest, FieldType::I32);
  Put(dest, ref);
});

EncoderClass(BladeI64Encoder, BladeI64, sizeof(BladeI64), {
  Put(dest, FieldType::I64);
  Put(dest, ref);
});

EncoderClass(BladeF64Encoder, BladeF64, sizeof(BladeF64), {
  Put(dest, FieldType::F64);
  Put(dest, ref);
});

EncoderClass(BladeStringEncoder, BladeString, ref.length() + sizeof(u32), {
  Put(dest, FieldType::String);
  Put(dest, static_cast<u32>(ref.length()));
  PutPtr(dest, ref.data(), ref.length());
});

EncoderClass(BladeNameEncoder, BladeName, ref.length() + sizeof(u32), {
  Put(dest, FieldType::BladeName);
  Put(dest, static_cast<u32>(ref.length()));
  PutPtr(dest, ref.data(), ref.length());
});

#undef EncoderClass

class BladeHeaderEncoder : public Encoder {
 public:
  explicit BladeHeaderEncoder(const BladeObject &ref) : ref(ref) {}

  void Encode(byte *dest) override {
    Put(dest, ref.header_);
    auto service_name = ref.GetServiceName();
    Put(dest, static_cast<u32>(service_name.length()));
    PutPtr(dest, service_name.data(), service_name.length());
  }

  size_t GetExpectedSize() override { return sizeof(RawBladeHeader) + sizeof(u32) + ref.GetServiceName().length(); }

 private:
  const BladeObject &ref;
};

}  // namespace blmesh