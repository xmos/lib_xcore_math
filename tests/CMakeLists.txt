cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(tests)

if(DEFINED XCORE_TARGET)
    set(APP_HW_TARGET ${XCORE_TARGET})
else()
    set(APP_HW_TARGET XK-EVK-XU316)
endif()


add_subdirectory(bfp_tests)
add_subdirectory(dct_tests)
add_subdirectory(fft_tests)
add_subdirectory(filter_tests)
add_subdirectory(scalar_tests)
add_subdirectory(vect_tests)
add_subdirectory(vpu_tests)
