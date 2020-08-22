//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.
#pragma once

#include <functional>
#include <unordered_map>

#include "blmesh/common.h"

namespace blmesh {

class NoSuchFieldException : public std::exception {};

class BladeObject {
 public:
  static Ptr<BladeObject> NewRequest(const std::string &service_name, const std::string &call_name);

  static Ptr<BladeObject> NewRequest(const std::string &service_name, u32 trace_id, const std::string &call_name);

  static Ptr<BladeObject> NewResponse(const std::string &service_name, const std::string &call_name);

  static Ptr<BladeObject> NewResponse(const std::string &service_name, u32 trace_id, const std::string &call_name);

  static Ptr<BladeObject> NewRegister(const std::string &service_name, const std::string &ip, Port port);

  static Ptr<BladeObject> NewRegister(const std::string &service_name, u32 trace_id, const std::string &ip, Port port);

  static Ptr<BladeObject> NewDeregister(const std::string &service_name, const std::string &ip);

  static Ptr<BladeObject> NewDeregister(const std::string &service_name, u32 trace_id, const std::string &ip);

  static Ptr<BladeObject> NewKeepAlive(const std::string &service_name);

  static Ptr<BladeObject> NewKeepAlive(const std::string &service_name, u32 trace_id);

  static Ptr<BladeObject> NewQuery(const std::string &service_name, const std::string &query_name);

  static Ptr<BladeObject> NewQuery(const std::string &service_name, u32 trace_id, const std::string &query_name);

  static Ptr<BladeObject> NewAnswer(const std::string &service_name);
  static Ptr<BladeObject> NewAnswer(const std::string &service_name, u32 trace_id);

  static Ptr<BladeObject> NewAnswer(const std::string &service_name, const std::string &query_name,
                                    const std::string &ip);

  static Ptr<BladeObject> NewAnswer(const std::string &service_name, u32 trace_id, const std::string &query_name,
                                    const std::string &ip);
  bool HasService();

  bool AddByte(const BladeName &key, BladeByte val);

  bool AddI32(const BladeName &key, BladeI32 val);

  bool AddI64(const BladeName &key, BladeI64 val);

  bool AddF64(const BladeName &key, BladeF64 val);

  bool AddString(const BladeName &key, const BladeString &val);

  bool RemoveField(const BladeName &key);

  FieldType GetFieldType(const BladeName &name) const;

  bool Contains(const BladeName &name) const;

  using IterateFunc = std::function<int(const BladeName &, FieldType, void *)>;
  int Iterate(IterateFunc f) const;

  u32 GetId() const;

  void SetId(u32 id);

  u32 GetTraceId() const;

  void SetTraceId(u32 trace_id);

  u32 GetType() const;

  void SetType(u32 type);

  u32 GetPayloadLength() const;

  u32 GetContentOffset() const;

  const BladeString &GetServiceName() const;

  void SetServiceName(const BladeString &service_name);

  BladeString GetString(const BladeName &key) const;

  BladeByte GetByte(const BladeName &key) const;

  BladeI32 GetI32(const BladeName &key) const;
  BladeI64 GetI64(const BladeName &key) const;

  BladeF64 GetF64(const BladeName &key) const;

  int Dsl(const std::string &str);

  void UpdateHeaderField() const;

  size_t GetEncodedSize() const;
  
  Ptr<BladeObject> Clone() const;
 protected:
  void SetPayloadLength(u32 payload_length);

  void SetContentOffset(u32 content_offset);

  void SetRawHeader(RawBladeHeader header);

  friend class BladeHeaderEncoder;
  friend class Opr;

 private:
  mutable RawBladeHeader header_;
  BladeString service_name_;

  template <typename ValueType>
  using Storage = std::unordered_map<BladeName, ValueType>;

  Storage<FieldType> name_;

  Storage<BladeByte> member_byte_;
  Storage<BladeI32> member_i32_;
  Storage<BladeI64> member_i64_;
  Storage<BladeF64> member_f64_;
  Storage<BladeString> member_string_;
};

}  // namespace blmesh