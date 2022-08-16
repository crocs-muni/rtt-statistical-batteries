# FIPS battery

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
