cc_binary(
  name = "chaos",
  srcs = [
    "chaos.cc",
    "data_manager.cc",
    "data_manager.h",
    "handlers.cc",
    "handlers.h",
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
