#!/bin/sh
set -e

pacman -S --noconfirm \
    ninja \
    mingw-w64-x86_64-nlohmann-json \
    mingw-w64-x86_64-gsl

# Configuration:
install_destination=$PWD/install
subdirectories=(
  bsi-src
  dieharder-src
  fips-src
  nist-sts-src
  testu01-src
)
build_type=Release

# Build:
for subdir in "${subdirectories[@]}"; do
  build_dir=$subdir/build

  # Cleanup
  if [ -d $build_dir ]; then
    /usr/bin/rm -rf $build_dir
  fi

  mkdir -p $subdir/build && cd $_
  cmake -G Ninja -DCMAKE_BUILD_TYPE=$build_type -DCMAKE_INSTALL_PREFIX=$install_destination ..
  ninja
  ninja install
  cd ../../
done
