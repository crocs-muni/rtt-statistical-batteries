include(zig.profile)

[settings]
os=Windows
arch=x86_64

[env]
CFLAGS=-target x86_64-windows-gnu
CXXFLAGS=-target x86_64-windows-gnu
