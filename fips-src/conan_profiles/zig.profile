include(default)

[settings]
compiler=clang
compiler.version=13
compiler.libcxx=libc++

[env]
AR=zig ar
ASM=zig cc
CC=zig cc
CXX=zig c++
RANLIB=zig ranlib
CONAN_CMAKE_GENERATOR=Ninja
