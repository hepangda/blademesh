cc_library(
  name = "framework",
  srcs = [
    "blade_object.cc",
    "blade_server.cc",
    "blade_session.cc",
    "client_session_policy.cc",
    "opr.cc",
    "service_session_policy.cc",
    "sync_session.cc",
  ],
  hdrs = [
    "client_session_policy.h",
    "session_policy.h",
    "decoder.h",
    "opr.h",
    "sync_session.h",
    "encoder.h",
    "blade_server.h",
    "blade_object.h",
    "blade_session.h",
    "service_session_policy.h",
    "buffer_adaptor.h",
  ],
  copts = [
    "-std=c++17",
  ],
  deps = [
    "//blmesh:common",
    "//blmesh:id_utils",
  ],
  visibility = [
    "//visibility:public",
  ],
)