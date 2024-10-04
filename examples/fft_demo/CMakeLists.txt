cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(fft_demo)

if(DEFINED XCORE_TARGET)
    set(APP_HW_TARGET ${XCORE_TARGET})
else()
    set(APP_HW_TARGET XK-EVK-XU316)
endif()

set(XMOS_SANDBOX_DIR ${CMAKE_CURRENT_LIST_DIR}/../../..)

include(${CMAKE_CURRENT_LIST_DIR}/../deps.cmake)

set(APP_COMPILER_FLAGS -Os)

if(NOT BUILD_NATIVE)
    list(APPEND APP_COMPILER_FLAGS -fxscope
                                   -report
                                   )
endif()

if(NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    list(APPEND APP_COMPILER_FLAGS -Werror
                                   -g
                                   )
else()
    list(APPEND APP_COMPILER_FLAGS # Suppress warning C4996: 'sprintf': This function or variable may be unsafe.
                                   # Consider using sprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
                                   # See online help for details.
                                   -D_CRT_SECURE_NO_WARNINGS
                                   # Suppress warning C5045: Compiler will insert Spectre mitigation for memory load if /wd5045 switch specified
                                   /wd5045
                                   )
endif()

if(PLATFORM_ID STREQUAL Linux OR PLATFORM_ID STREQUAL Darwin)
    list(APPEND APP_COMPILER_FLAGS -lm)
endif()

XMOS_REGISTER_APP()