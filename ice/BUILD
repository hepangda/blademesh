cc_binary(
  name = "ice",
  srcs = [
    "ice.cc",
    "service_manager.cc",
    "service_manager.h",
  ],
  copts = [
    "-std=c++17",
    "-lglog",
    "-lpthread",
    "-lgflags",
  ],
  linkopts = [
    "-lglog",
    "-pthread",
    "-lgflags",
  ],
  deps = [
    "//blade:framework",
    "//blmesh:noncopyable",
    "//blmesh:rw_lock",
  ],
)
