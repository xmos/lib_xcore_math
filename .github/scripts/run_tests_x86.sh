#!/bin/bash
set -e

./build.x86/test/scalar_tests/scalar_tests

./build.x86/test/vect_tests/vect_tests

./build.x86/test/bfp_tests/bfp_tests

./build.x86/test/fft_tests/fft_tests

./build.x86/test/filter_tests/filter_tests