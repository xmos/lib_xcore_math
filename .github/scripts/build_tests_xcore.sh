#!/bin/bash
set -e

cmake -B build.xcore -DSMOKE_TEST=ON -DCMAKE_TOOLCHAIN_FILE=etc/xmos_toolchain.cmake
cmake --build build.xcore
