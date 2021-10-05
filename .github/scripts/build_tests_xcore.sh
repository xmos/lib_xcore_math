#!/bin/bash
set -e

pwd

cmake -B build.xcore -DSMOKE_TEST=ON -DGEN_FFT_LUT=OFF -DCMAKE_TOOLCHAIN_FILE=etc/xmos_toolchain.cmake
cmake --build build.xcore

cp build.xcore/test/vect_tests/vect_tests.xe /artifacts
cp build.xcore/test/bfp_tests/bfp_tests.xe /artifacts
cp build.xcore/test/scalar_tests/scalar_tests.xe /artifacts
cp build.xcore/test/fft_tests/fft_tests.xe /artifacts
cp build.xcore/test/filter_tests/filter_tests.xe /artifacts