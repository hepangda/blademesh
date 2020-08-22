# https://docs.bazel.build/versions/master/be/c-cpp.html#cc_library
cc_library(
  name = "framework",
  srcs = [
    "blade_object.cc",
  ],
  hdrs = [
    "blade_object.h"
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