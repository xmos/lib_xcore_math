cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(lib_xcore_math_examples)

add_subdirectory(bfp_demo)
add_subdirectory(app_fft_demo)
add_subdirectory(app_filter_demo)
add_subdirectory(app_vect_demo)
