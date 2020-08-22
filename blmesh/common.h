//  Copyright (c) hepangda
//
//  This source code is licensed under the MIT license found in the
//  LICENSE file in the root directory of this source tree.

#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>

#include "blmesh/log.h"

#include "3rdparty/asio/asio.hpp"

namespace blmesh {

using byte = std::byte;

using i16 = std::int16_t;
using u16 = std::uint16_t;

using i32 = std::int32_t;
using u32 = std::uint32_t;

using i64 = std::int64_t;
using u64 = std::uint64_t;

using f32 = float;
using f64 = double;

using BladeName = std::string;
using BladeString = std::string;

using BladeByte = byte;
using BladeI32 = i32;
using BladeI64 = i64;
using BladeF64 = f64;

enum class FieldType : u16 {
  None = 0x0000,
  Byte = 0x0002,
  I32 = 0x0012,
  I64 = 0x0013,
  F64 = 0x0021,
  String = 0x00A0,
  BladeName = 0x00A1,
};

enum BladeType : u32 {
  kBTRequest = 0x00'00'00'00,
  kBTResponse = 0x00'00'00'01,
  kBTRegister = 0x00'00'00'10,
  kBTDeregister = 0x00'00'00'11,
  kBTKeepAlive = 0x00'00'00'20,
  kBTQuery = 0x00'00'00'A0,
  kBTAnswer = 0x00'00'00'A1,
};

constexpr u32 kBladeMagicNumber = 0x01E0ADB1;

struct RawBladeHeader {
  i32 magic_number{kBladeMagicNumber};
  u32 id{0};
  u32 trace_id{0};
  u32 type{0};
  u32 payload_length{0};
  u32 content_offset{0};
  u32 extra[4]{0};
  byte service_name[0];
};

class UnreachableCode : public std::exception {};

template <typename T>
using Rc = std::shared_ptr<T>;

template <typename T>
using Ptr = std::unique_ptr<T>;

using Port = unsigned short;

constexpr Port kKartosPort = 13520;
constexpr Port kLokiPort = 13521;
constexpr Port kIcePort = 13522;
constexpr Port kChaosPort = 13523;

constexpr const char *kKartosServiceName = "#KARTOS";
constexpr const char *kLokiServiceName = "#LOKI";
constexpr const char *kWatchdogServiceName = "#WD";
constexpr const char *kIceServiceName = "#ICE";
constexpr const char *kIceRegisterServiceName = "#ICE_SR";
constexpr const char *kCallerServiceName = "#ANY_CALL";

constexpr const char *kRequestService = "#CALL_NAME";
constexpr const char *kResponseService = "#RESP_NAME";
constexpr const char *kRegisterIp = "#REG_IP";
constexpr const char *kRegisterPort = "#REG_PORT";
constexpr const char *kIceOk = "#ICE_OK";
constexpr const char *kIceFailed = "#ICE_FAIL";

constexpr const char *kQuery = "#QUERY";
constexpr const char *kAnswerIp = "#ANS_IP";
constexpr const char *kAnswerPort = "#ANS_PORT";
constexpr const char *kNoAnswer = "#NO_ANS";
constexpr const char *kNoService = "#NO_SRV";

constexpr const char *kChaosReport = "#REPORT";
constexpr const char *kChaosSrvQuery = "#CHAOS_SERVICE_QUERIES";
constexpr const char *kChaosSrvError = "#CHAOS_SERVICE_ERRORS";
constexpr const char *kChaosSrvShutdown = "#CHAOS_SERVICE_SHUTDOWNS";
constexpr const char *kChaosSrvAvgBytes = "#CHAOS_SERVICE_AVGBYTES";
constexpr const char *kChaosMostQuery = "#CHAOS_MOST_QUERIES";
constexpr const char *kChaosMostError = "#CHAOS_MOST_ERRORS";
constexpr const char *kChaosMostShutdown = "#CHAOS_MOST_SHUTDOWNS";
constexpr const char *kChaosAvaSrv = "#CHAOS_AVA_SERVICE";
constexpr const char *kChaosAvaSrvN = "#CHAOS_AVA_SRV_NUMS";

constexpr const char *kChaosReportType = "#CHAOS_RT";
constexpr const char *kChaosName = "#CHAOS_NAME";
constexpr const char *kChaosVal = "#CHAOS_VAL";
constexpr const char *kChaosOk = "#CHAOS_OK";

constexpr BladeByte kZeroByte = static_cast<BladeByte>(0);

namespace net = asio;

namespace nets {

using IoContext = net::io_context;
using ErrorCode = net::error_code;

using IpAddress = net::ip::address;
using IpAddressV4 = net::ip::address_v4;
using IpAddressV6 = net::ip::address_v6;

using TcpAcceptor = net::ip::tcp::acceptor;
using TcpEndpoint = net::ip::tcp::endpoint;
using TcpIoStream = net::ip::tcp::iostream;
using TcpResolver = net::ip::tcp::resolver;
using TcpSocket = net::ip::tcp::socket;

using UdpEndpoint = net::ip::tcp::endpoint;
using UdpResolver = net::ip::udp::resolver;
using UdpSocket = net::ip::udp::socket;

using ConstBuffer = net::const_buffer;
using MutableBuffer = net::mutable_buffer;
using StreamBuf = net::streambuf;

template <typename... Args>
inline decltype(auto) MakeAddress(Args &&... addr_str) {
  return net::ip::make_address(std::forward<Args &&>(addr_str)...);
}

template <typename... Args>
inline decltype(auto) MakeTcpEndpoint(Port port, Args &&... addr_str) {
  return TcpEndpoint{MakeAddress(std::forward<Args &&>(addr_str)...), port};
}

inline decltype(auto) MakeTcpEndpoint(Port port) { return TcpEndpoint{MakeAddress("0.0.0.0"), port}; }

#define NetReadHandler(ec_name, byt_name) (const nets::ErrorCode &ec_name, size_t byt_name)
#define NetWriteHandler(ec_name, byt_name) (const nets::ErrorCode &ec_name, size_t byt_name)
#define NetMoveAcceptHandler(ec_name, socket_name) (const nets::ErrorCode &ec_name, nets::TcpSocket socket_name)
#define NetConnectHandler(ec_name) (const nets::ErrorCode &ec_name)
#define NetTimerHandler(ec_name) (const nets::ErrorCode &ec_name)
#define Service(obj) (Ptr<BladeObject> & obj)->Ptr<BladeObject>
}  // namespace nets

}  // namespace blmesh

#ifndef RELEASE_VERSION

#include <iostream>

#define DebugPrint(sth) std::cout << "<Debug> " << sth << "\n"
#define DebugDisplay(title, show) std::cout << "<Debug>  " << title << " = " << show << "\n"

#else

#define DebugPrint(sth)
#define DebugDisplay(title, show)

#endif  // RELEASE_VERSION