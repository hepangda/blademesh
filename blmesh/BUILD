cc_library(
  name = "common",
  srcs = [
    "log.cc"
  ],
  hdrs = [
    "log.h",
    "common.h",
  ],
  includes = [
    "."
  ],
  copts = [
    "-std=c++17",
  ],
  deps = [
    "//3rdparty/asio:asio",
  ],
  visibility = [
    "//visibility:public",
  ],
)

cc_library(
  name = "id_utils",
  srcs = [
    "id.cc"
  ],
  hdrs = [
    "id.h",
  ],
  includes = [
    "."
  ],
  copts = [
    "-std=c++17",
  ],
  deps = [
    ":common",
  ],
  visibility = [
    "//visibility:public",
  ],
)

cc_library(
  name = "rw_lock",
  srcs = [],
  hdrs = [
    "rw_lock.h"
  ],
  includes = [
    "."
  ],
  copts = [
    "-std=c++17",
  ],
  deps = [
    ":noncopyable"
  ],
  visibility = [
    "//visibility:public",
  ],
)

cc_library(
  name = "noncopyable",
  srcs = [],
  hdrs = [
    "noncopyable.h"
  ],
  includes = [
    "."
  ],
  copts = [
    "-std=c++17",
  ],
  deps = [],
  visibility = [
    "//visibility:public",
  ],
)