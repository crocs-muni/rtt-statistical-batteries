# BSI battery

## MinGW dependencies:
```bash
pacman -S mingw-w64-x86_64-nlohmann-json ninja
```

## Compilation
```bash
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=<dir_where_to_install_fips_binary>..
ninja
ninja install
```

## Compilation with zig
Zig compiler was used to compile the source on Linux for various platforms: windows, linux, macos
It can be downloaded from its [official download page](https://ziglang.org/download/).
Zig is a standalone binary which can be added into a `PATH` environment variable under a Linux system
and can be easily used from there.
Take a look at `buildall.sh` script to find out how it's done.
