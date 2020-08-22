//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include "opr.h"

#include "blade/blade_object.h"
#include "blade/decoder.h"
#include "blade/encoder.h"

namespace blmesh::Opr {

RawBladeHeader GetRawBladeHeaderFromPtr(const void *ptr) {
  return *reinterpret_cast<RawBladeHeader *>(const_cast<void *>(ptr));
}

int Encode(const BladeObject &object, void *dest, int length_limit) {
  int ret = 0;
  byte *dest_now = static_cast<byte *>(dest);

  object.UpdateHeaderField();
  BladeHeaderEncoder bh_enc{object};
  if (bh_enc.GetExpectedSize() > length_limit) {
    return 0;
  }
  bh_enc.Encode(dest_now);
  bh_enc.MoveDestAndRet(dest_now, ret);

  int length_remain = length_limit - bh_enc.GetExpectedSize();
  ret += object.Iterate([&dest_now, &length_remain](const BladeName &name, FieldType type, void *origin) {
    BladeNameEncoder name_enc{name};
    int ret = 0;
    switch (type) {
#define simple_encode(type)                                                   \
  case FieldType::type: {                                                     \
    auto it = GetObjectFromPtr<Blade##type>(origin);                          \
    Blade##type##Encoder enc{it};                                             \
    if (name_enc.GetExpectedSize() + enc.GetExpectedSize() > length_remain) { \
      return 0;                                                               \
    }                                                                         \
    name_enc.Encode(dest_now);                                                \
    name_enc.MoveDestAndRet(dest_now, ret);                                   \
    enc.Encode(dest_now);                                                     \
    enc.MoveDestAndRet(dest_now, ret);                                        \
    break;                                                                    \
  }
      simple_encode(Byte) simple_encode(I32) simple_encode(I64) simple_encode(F64) simple_encode(String) default
          : throw UnreachableCode{};
#undef simple_encode
    }

    return ret;
  });

  return ret;
}

Ptr<BladeObject> DecodeWithHeader(RawBladeHeader header, void *origin_v, int raw_length) {
  auto origin = static_cast<byte *>(origin_v);

  if (raw_length < header.payload_length) {
    return nullptr;
  }

  Decoder dec{origin, raw_length};

  auto ret = std::make_unique<BladeObject>();
  ret->SetRawHeader(header);

  auto service_name = dec.TakeBladeString();
  ret->SetServiceName(service_name);

  while (dec.NotEnd()) {
    try {
      auto field_type = dec.Take<FieldType>();
      if (field_type != FieldType::BladeName) {
        return nullptr;
      }

      auto name = dec.TakeBladeString();
      field_type = dec.Take<FieldType>();
      switch (field_type) {
#define simple_decode(type)           \
  case FieldType::type: {             \
    auto val = dec.TakeBlade##type(); \
    ret->Add##type(name, val);        \
    break;                            \
  }
        simple_decode(Byte);
        simple_decode(I32);
        simple_decode(I64);
        simple_decode(F64);
        simple_decode(String);

      default:
        throw UnreachableCode{};
#undef simple_decode
      }
    } catch (RemainLengthNotEnoughException &) {
      return nullptr;
    }
  }

  return ret;
}

Ptr<BladeObject> DecodeWithHeader(RawBladeHeader header, const void *origin_v, int raw_length) {
  return DecodeWithHeader(header, const_cast<void *>(origin_v), raw_length);
}

Ptr<BladeObject> Decode(void *origin_v, int raw_length) {
  auto origin = static_cast<byte *>(origin_v);

  Decoder dec{origin, raw_length};
  if (!dec.MinSizeSatisfied(raw_length)) {
    return nullptr;
  }

  auto header = dec.Take<RawBladeHeader>();
  return DecodeWithHeader(header, static_cast<void *>(origin + sizeof(RawBladeHeader)), raw_length - sizeof(RawBladeHeader));
}

Ptr<BladeObject> Decode(const void *origin_v, int raw_length) {
  return Decode(const_cast<void *>(origin_v), raw_length);
}

}  // namespace blmesh::Opr