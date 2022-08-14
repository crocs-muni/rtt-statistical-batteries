include(zig.profile)

[settings]
os=Linux
arch=x86_64

[env]
CFLAGS=-target x86_64-linux-musl
CXXFLAGS=-target x86_64-linux-musl
