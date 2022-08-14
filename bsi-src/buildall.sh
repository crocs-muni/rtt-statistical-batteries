#!/bin/bash

set -e

clean_build_folder () {
  build_folder=$1
  if [ -d $build_folder ]; then
    /usr/bin/rm -rf $build_folder
  fi
}

build_architectures=(linux_x64 macos_arm64 macos_x64 windows_x64)

for arch in "${build_architectures[@]}"
do
  bf="build_${arch}"
  clean_build_folder ${bf}

  mkdir -p ${bf}

  cd ${bf}

  conan install .. -pr:b ../conan_profiles/zig.profile -pr:h ../conan_profiles/zig_${arch}.profile -b missing

  cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/toolchain/zig_${arch}.cmake -DCMAKE_BUILD_TYPE=Release ..

  cmake --build . --config Release -j`nproc` --verbose

  cd ..
done
