//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#include "handlers.h"

#include "chaos/data_manager.h"

namespace blmesh {

Ptr<BladeObject> DoReport(Ptr<BladeObject> &obj) {
  auto ret = BladeObject::NewResponse(kChaosReport, obj->GetTraceId(), obj->GetString(kRequestService));

  try {
    auto type = obj->GetI64(kChaosReportType);

    switch (type) {
    case 1:  // Query
      obj->Iterate([](const BladeName &name, FieldType t, void *p) -> int {
        if (t == FieldType::I32) {
          DataManager::Get().DoQuery(name, *reinterpret_cast<int *>(p));
        }
        return 0;
      });
      break;
    case 2:  // Error
      obj->Iterate([](const BladeName &name, FieldType t, void *p) -> int {
        if (t == FieldType::I32) {
          DataManager::Get().DoError(name, *reinterpret_cast<int *>(p));
        }
        return 0;
      });
      break;
    case 3:  // Shutdowns
      obj->Iterate([](const BladeName &name, FieldType t, void *p) -> int {
        if (t == FieldType::I32) {
          DataManager::Get().DoShutdown(name, *reinterpret_cast<int *>(p));
        }
        return 0;
      });
      break;
    case 4:  // Bytes
      obj->Iterate([](const BladeName &name, FieldType t, void *p) -> int {
        if (t == FieldType::I32) {
          DataManager::Get().DoBytes(name, *reinterpret_cast<int *>(p));
        }
        return 0;
      });
      break;
    }

    ret->AddByte(kChaosOk, kZeroByte);
  } catch (...) {
  }

  return ret;
}

Ptr<BladeObject> DoQuery(Ptr<BladeObject> &obj) {
  auto ret = BladeObject::NewResponse(kChaosSrvQuery, obj->GetTraceId(), obj->GetString(kRequestService));

  try {
    auto name = obj->GetString(kChaosName);
    auto res = DataManager::Get().GetQuery(name);
    ret->AddI32(name, res);
    ret->AddByte(kChaosOk, kZeroByte);
  } catch (...) {
  }

  return ret;
}

Ptr<BladeObject> DoError(Ptr<BladeObject> &obj) {
  auto ret = BladeObject::NewResponse(kChaosSrvError, obj->GetTraceId(), obj->GetString(kRequestService));

  try {
    auto name = obj->GetString(kChaosName);
    auto res = DataManager::Get().GetError(name);
    ret->AddI32(name, res);
    ret->AddByte(kChaosOk, kZeroByte);
  } catch (...) {
  }

  return ret;
}

Ptr<BladeObject> DoShutdown(Ptr<BladeObject> &obj) {
  auto ret = BladeObject::NewResponse(kChaosSrvShutdown, obj->GetTraceId(), obj->GetString(kRequestService));

  try {
    auto name = obj->GetString(kChaosName);
    auto res = DataManager::Get().GetShutdown(name);
    ret->AddI32(name, res);
    ret->AddByte(kChaosOk, kZeroByte);
  } catch (...) {
  }

  return ret;
}

Ptr<BladeObject> DoBytes(Ptr<BladeObject> &obj) {
  auto ret = BladeObject::NewResponse(kChaosSrvAvgBytes, obj->GetTraceId(), obj->GetString(kRequestService));

  try {
    auto name = obj->GetString(kChaosName);
    auto res = DataManager::Get().GetBytes(name);
    ret->AddI32(name, res);
    ret->AddByte(kChaosOk, kZeroByte);
  } catch (...) {
  }

  return ret;
}

Ptr<BladeObject> DoMostQuery(Ptr<BladeObject> &obj) {
  auto ret = BladeObject::NewResponse(kChaosMostQuery, obj->GetTraceId(), obj->GetString(kRequestService));

  try {
    auto res = DataManager::Get().GetMostQuery();
    if (res != nullptr) {
      ret->AddString(kChaosName, res->first);
      ret->AddI32(kChaosVal, res->second);
    } else {
      ret->AddByte(kNoAnswer, kZeroByte);
    }
    ret->AddByte(kChaosOk, kZeroByte);
  } catch (...) {
  }

  return ret;
}

Ptr<BladeObject> DoMostError(Ptr<BladeObject> &obj) {
  auto ret = BladeObject::NewResponse(kChaosMostError, obj->GetTraceId(), obj->GetString(kRequestService));

  try {
    auto res = DataManager::Get().GetMostError();
    if (res != nullptr) {
      ret->AddString(kChaosName, res->first);
      ret->AddI32(kChaosVal, res->second);
    } else {
      ret->AddByte(kNoAnswer, kZeroByte);
    }
    ret->AddByte(kChaosOk, kZeroByte);
  } catch (...) {
  }

  return ret;
}

Ptr<BladeObject> DoMostShutdown(Ptr<BladeObject> &obj) {
  auto ret = BladeObject::NewResponse(kChaosMostShutdown, obj->GetTraceId(), obj->GetString(kRequestService));

  try {
    auto res = DataManager::Get().GetMostShutdown();
    if (res != nullptr) {
      ret->AddString(kChaosName, res->first);
      ret->AddI32(kChaosVal, res->second);
    } else {
      ret->AddByte(kNoAnswer, kZeroByte);
    }
    ret->AddByte(kChaosOk, kZeroByte);
  } catch (...) {
  }

  return ret;
}

}  // namespace blmesh