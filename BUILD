load("@rules_cc//cc:defs.bzl", "cc_binary")

cc_binary(
   name = "stoyozipxx",
   srcs = (
      "src/compressor.cpp",
      "src/decompressor.cpp",
      "src/main.cpp",
      "src/tester.cpp",
      "src/buffer.hpp",
      "src/compressor.hpp",
      "src/decompressor.hpp",
      "src/oligorithm.hpp",
      "src/tester.hpp",
   ),
   deps = (
      "@boost//:circular_buffer",
      "@boost//:program_options",
   ),
   copts = (
      "/std:c++17",
   )
)
