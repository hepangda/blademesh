//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include "blade_object.h"

#include <cstring>

#include <sstream>

#include "blmesh/id.h"
#include "blmesh/log.h"

namespace blmesh {

Ptr<BladeObject> BladeObject::NewRequest(const std::string &service_name, const std::string &call_name) {
  auto ret = std::make_unique<BladeObject>();
  ret->SetType(kBTRequest);
  ret->SetServiceName(service_name);
  ret->AddString(kRequestService, call_name);
  ret->SetId(id::Generate());
  ret->SetTraceId(ret->GetTraceId());
  return ret;
}

Ptr<BladeObject> BladeObject::NewRequest(const std::string &service_name, u32 trace_id, const std::string &call_name) {
  auto ret = NewRequest(service_name, call_name);
  ret->SetTraceId(trace_id);
  return ret;
}

Ptr<BladeObject> BladeObject::NewResponse(const std::string &service_name, const std::string &call_name) {
  auto ret = std::make_unique<BladeObject>();
  ret->SetType(kBTResponse);
  ret->SetServiceName(service_name);
  ret->AddString(kResponseService, call_name);
  ret->SetId(id::Generate());
  return ret;
}

Ptr<BladeObject> BladeObject::NewResponse(const std::string &service_name, u32 trace_id, const std::string &call_name) {
  auto ret = NewResponse(service_name, call_name);
  ret->SetTraceId(trace_id);
  return ret;
}

Ptr<BladeObject> BladeObject::NewRegister(const std::string &service_name, const std::string &ip, Port port) {
  auto ret = std::make_unique<BladeObject>();
  ret->SetType(kBTRegister);
  ret->SetId(id::Generate());
  ret->SetServiceName(service_name);
  ret->AddString(kRegisterIp, ip);
  ret->AddI32(kRegisterPort, port);
  ret->AddString(kRequestService, kIceRegisterServiceName);

  return ret;
}

Ptr<BladeObject> BladeObject::NewRegister(const std::string &service_name, u32 trace_id, const std::string &ip,
                                          Port port) {
  auto ret = NewRegister(service_name, ip, port);
  ret->SetTraceId(trace_id);
  return ret;
}

Ptr<BladeObject> BladeObject::NewDeregister(const std::string &service_name, const std::string &ip) {
  auto ret = std::make_unique<BladeObject>();
  ret->SetType(kBTDeregister);
  ret->SetId(id::Generate());
  ret->SetServiceName(service_name);
  ret->AddString(kRegisterIp, ip);
  ret->AddString(kRequestService, kIceRegisterServiceName);
  return ret;
}

Ptr<BladeObject> BladeObject::NewDeregister(const std::string &service_name, u32 trace_id, const std::string &ip) {
  auto ret = NewDeregister(service_name, ip);
  ret->SetTraceId(trace_id);
  return ret;
}

Ptr<BladeObject> BladeObject::NewKeepAlive(const std::string &service_name) {
  auto ret = std::make_unique<BladeObject>();
  ret->SetType(kBTKeepAlive);
  ret->SetId(id::Generate());
  ret->SetServiceName(service_name);
  return ret;
}

Ptr<BladeObject> BladeObject::NewKeepAlive(const std::string &service_name, u32 trace_id) {
  auto ret = NewKeepAlive(service_name);
  ret->SetTraceId(trace_id);
  return ret;
}

Ptr<BladeObject> BladeObject::NewQuery(const std::string &service_name, const std::string &query_name) {
  auto ret = std::make_unique<BladeObject>();
  ret->SetType(kBTQuery);
  ret->SetId(id::Generate());
  ret->SetServiceName(service_name);
  ret->AddString(kRequestService, kIceServiceName);
  ret->AddString(kQuery, query_name);
  return ret;
}

Ptr<BladeObject> BladeObject::NewQuery(const std::string &service_name, u32 trace_id, const std::string &query_name) {
  auto ret = NewQuery(service_name, query_name);
  ret->SetTraceId(trace_id);
  return ret;
}

Ptr<BladeObject> BladeObject::NewAnswer(const std::string &service_name) {
  auto ret = std::make_unique<BladeObject>();
  ret->SetType(kBTAnswer);
  ret->SetId(id::Generate());
  ret->SetServiceName(service_name);
  ret->AddByte(kNoAnswer, static_cast<BladeByte>(0));
  return ret;
}

Ptr<BladeObject> BladeObject::NewAnswer(const std::string &service_name, u32 trace_id) {
  auto ret = NewAnswer(service_name);
  ret->SetTraceId(trace_id);
  return ret;
}

Ptr<BladeObject> BladeObject::NewAnswer(const std::string &service_name, const std::string &query_name,
                                        const std::string &ip) {
  auto ret = std::make_unique<BladeObject>();
  ret->SetType(kBTAnswer);
  ret->SetId(id::Generate());
  ret->SetServiceName(service_name);
  ret->AddString(kAnswerIp, ip);
  return ret;
}

Ptr<BladeObject> BladeObject::NewAnswer(const std::string &service_name, u32 trace_id, const std::string &query_name,
                                        const std::string &ip) {
  auto ret = NewAnswer(service_name, query_name, ip);
  ret->SetTraceId(trace_id);
  return ret;
}

bool BladeObject::HasService() { return !Contains(kNoService); }

bool BladeObject::AddByte(const BladeName &key, BladeByte val) {
  if (Contains(key)) {
    return false;
  }

  name_.emplace(key, FieldType::Byte);
  member_byte_.emplace(key, val);
  return true;
}

bool BladeObject::AddI32(const BladeName &key, BladeI32 val) {
  if (Contains(key)) {
    return false;
  }

  name_.emplace(key, FieldType::I32);
  member_i32_.emplace(key, val);
  return true;
}

bool BladeObject::AddI64(const BladeName &key, BladeI64 val) {
  if (Contains(key)) {
    return false;
  }

  name_.emplace(key, FieldType::I64);
  member_i64_.emplace(key, val);
  return true;
}

bool BladeObject::AddF64(const BladeName &key, BladeF64 val) {
  if (Contains(key)) {
    return false;
  }

  name_.emplace(key, FieldType::F64);
  member_f64_.emplace(key, val);
  return true;
}

bool BladeObject::AddString(const BladeName &key, const BladeString &val) {
  if (Contains(key)) {
    return false;
  }

  name_.emplace(key, FieldType::String);
  member_string_.emplace(key, val);
  return true;
}

bool BladeObject::RemoveField(const BladeName &key) {
  if (!Contains(key)) {
    return false;
  }

  FieldType type = GetFieldType(key);
  switch (type) {
    case FieldType::Byte:
      member_byte_.erase(key);
      break;
    case FieldType::I32:
      member_i32_.erase(key);
      break;
    case FieldType::I64:
      member_i64_.erase(key);
      break;
    case FieldType::F64:
      member_f64_.erase(key);
      break;
    case FieldType::String:
      member_string_.erase(key);
      break;
    default:
      throw UnreachableCode{};
  }

  name_.erase(key);
  return true;
}

FieldType BladeObject::GetFieldType(const BladeName &name) const {
  auto iter = name_.find(name);
  return (iter != name_.end()) ? iter->second : FieldType::None;
}

bool BladeObject::Contains(const BladeName &name) const { return name_.find(name) != name_.end(); }

// Callable prototype: int(const BladeName &, FieldType, void *)
int BladeObject::Iterate(BladeObject::IterateFunc f) const {
  int ret = 0;
  auto iter_type = [&ret, &f, this](auto &members, auto type_name) {
    for (auto i : members) {
      int this_ret = f(i.first, type_name, &i.second);
      if (this_ret == 0) {
        return 0;
      }
      ret += this_ret;
    }
    return ret;
  };

  iter_type(member_byte_, FieldType::Byte);
  iter_type(member_i32_, FieldType::I32);
  iter_type(member_i64_, FieldType::I64);
  iter_type(member_f64_, FieldType::F64);
  iter_type(member_string_, FieldType::String);

  return ret;
}

u32 BladeObject::GetId() const { return header_.id; }

void BladeObject::SetId(u32 id) { header_.id = id; }

u32 BladeObject::GetTraceId() const { return header_.trace_id; }

void BladeObject::SetTraceId(u32 trace_id) { header_.trace_id = trace_id; }

u32 BladeObject::GetType() const { return header_.type; }

void BladeObject::SetType(u32 type) { header_.type = type; }

u32 BladeObject::GetPayloadLength() const { return header_.payload_length; }

u32 BladeObject::GetContentOffset() const { return header_.content_offset; }

const BladeString &BladeObject::GetServiceName() const { return service_name_; }

void BladeObject::SetServiceName(const BladeString &service_name) { service_name_ = service_name; }

BladeString BladeObject::GetString(const BladeName &key) const {
  if (GetFieldType(key) != FieldType::String) {
    throw NoSuchFieldException{};
  }

  return member_string_.at(key);
}

BladeByte BladeObject::GetByte(const BladeName &key) const {
  if (GetFieldType(key) != FieldType::Byte) {
    throw NoSuchFieldException{};
  }

  return member_byte_.at(key);
}

BladeI32 BladeObject::GetI32(const BladeName &key) const {
  if (GetFieldType(key) != FieldType::I32) {
    throw NoSuchFieldException{};
  }

  return member_i32_.at(key);
}

BladeI64 BladeObject::GetI64(const BladeName &key) const {
  if (GetFieldType(key) != FieldType::I64) {
    throw NoSuchFieldException{};
  }

  return member_i64_.at(key);
}

BladeF64 BladeObject::GetF64(const BladeName &key) const {
  if (GetFieldType(key) != FieldType::F64) {
    throw NoSuchFieldException{};
  }

  return member_f64_.at(key);
}

int BladeObject::Dsl(const std::string &str) {
  using namespace std::literals;
  using Strategy = std::function<int(std::stringstream &, BladeObject *)>;
#define strategy_func [](std::stringstream & ss, BladeObject * self) -> int
  std::unordered_map<std::string, Strategy> strategy_map{
      {"id"s,
       [](std::stringstream &ss, BladeObject *self) -> int {
         u32 id;
         ss >> id;
         if (ss.bad()) {
           return 1;
         }
         self->SetId(id);
         return 0;
       }},
      {"trace_id"s,
       [](std::stringstream &ss, BladeObject *self) -> int {
         u32 trace_id;
         ss >> trace_id;
         if (ss.bad()) {
           return 1;
         }
         self->SetTraceId(trace_id);
         return 0;
       }},
      {"type"s,
       [](std::stringstream &ss, BladeObject *self) -> int {
         u32 type;
         ss >> type;
         if (ss.bad()) {
           return 1;
         }
         self->SetType(type);
         return 0;
       }},
      {"service_name"s,
       [](std::stringstream &ss, BladeObject *self) -> int {
         BladeString service_name;
         ss >> service_name;
         if (ss.bad()) {
           return 1;
         }
         self->SetServiceName(service_name);
         return 0;
       }},
      {"byte"s,
       [](std::stringstream &ss, BladeObject *self) -> int {
         BladeString name;
         int8_t byt;
         ss >> name >> byt;
         if (ss.bad()) {
           return 1;
         }
         self->AddByte(name, static_cast<BladeByte>(byt));
         return 0;
       }},
      {"i32"s,
       [](std::stringstream &ss, BladeObject *self) -> int {
         BladeString name;
         BladeI32 val;
         ss >> name >> val;
         if (ss.bad()) {
           return 1;
         }
         self->AddI32(name, val);
         return 0;
       }},
      {"i64"s,
       [](std::stringstream &ss, BladeObject *self) -> int {
         BladeString name;
         BladeI64 val;
         ss >> name >> val;
         if (ss.bad()) {
           return 1;
         }
         self->AddI64(name, val);
         return 0;
       }},
      {"f64"s,
       [](std::stringstream &ss, BladeObject *self) -> int {
         BladeString name;
         BladeF64 val;
         ss >> name >> val;
         if (ss.bad()) {
           return 1;
         }
         self->AddF64(name, val);
         return 0;
       }},
      {"string"s,
       [](std::stringstream &ss, BladeObject *self) -> int {
         BladeString name;
         u32 length;
         ss >> name >> length;
         if (ss.bad()) {
           return 1;
         }
         auto val = std::make_unique<char[]>(length + 1);
         ss.ignore();
         ss.read(val.get(), length);
         val[length] = '\0';
         if (ss.bad()) {
           return 1;
         }

         self->AddString(name, val.get());
         return 0;
       }},
      {"str"s,
       [](std::stringstream &ss, BladeObject *self) -> int {
         BladeString name;
         BladeString val;
         ss >> name >> val;
         if (ss.bad()) {
           return 1;
         }

         self->AddString(name, val);
         return 0;
       }},
      {"remove"s,
       [](std::stringstream &ss, BladeObject *self) -> int {
         BladeName name;
         ss >> name;
         if (ss.bad()) {
           return 1;
         }
         self->RemoveField(name);
         return 0;
       }},
  };
#undef strategy_func
  int ret_code = 0;

  std::stringstream ss;
  ss.write(str.data(), str.length());

  std::string opr_name;
  while (ss >> opr_name) {
    if (strategy_map.find(opr_name) != strategy_map.end()) {
      ret_code += strategy_map[opr_name](ss, this);
    } else {
      return -1;
    }
  }

  return ret_code;
}

void BladeObject::UpdateHeaderField() const {
  int payload_length = Iterate([](const BladeName &name, FieldType type, void *field) {
    auto base = 2 * sizeof(FieldType) + sizeof(u32) + name.length();
    switch (type) {
      case FieldType::Byte:
        return base + sizeof(BladeByte);
      case FieldType::I32:
        return base + sizeof(BladeI32);
      case FieldType::I64:
        return base + sizeof(BladeI64);
      case FieldType::F64:
        return base + sizeof(BladeF64);
      case FieldType::String:
        return base + sizeof(u32) + reinterpret_cast<BladeString *>(field)->length();
      default:
        throw UnreachableCode{};
    }
  });

  header_.payload_length = payload_length + sizeof(u32) + service_name_.length();
  header_.content_offset = sizeof(RawBladeHeader) + sizeof(u32) + service_name_.length();
}

size_t BladeObject::GetEncodedSize() const {
  UpdateHeaderField();
  return sizeof(RawBladeHeader) + header_.payload_length;
}

Ptr<BladeObject> BladeObject::Clone() const {
  auto ret = std::make_unique<BladeObject>();
  *ret = *this;
  return ret;
}

void BladeObject::SetPayloadLength(u32 payload_length) { header_.payload_length = payload_length; }

void BladeObject::SetContentOffset(u32 content_offset) { header_.content_offset = content_offset; }

void BladeObject::SetRawHeader(RawBladeHeader header) { header_ = header; }

}  // namespace blmesh