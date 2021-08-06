#!/bin/bash
set -e

cmake -B build -DCMAKE_TOOLCHAIN_FILE=etc/xmos_toolchain.cmake
cmake --build build
