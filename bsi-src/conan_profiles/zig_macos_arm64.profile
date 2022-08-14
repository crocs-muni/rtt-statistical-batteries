include(zig.profile)

[settings]
os=Macos
arch=armv8

[env]
CFLAGS=-target aarch64-macos-none
CXXFLAGS=-target aarch64-macos-none
