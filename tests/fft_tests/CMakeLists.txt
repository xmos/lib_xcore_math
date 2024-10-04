cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(fft_tests)

if(DEFINED XCORE_TARGET)
    set(APP_HW_TARGET ${XCORE_TARGET})
else()
    set(APP_HW_TARGET XK-EVK-XU316)
endif()

set(XMOS_SANDBOX_DIR ${CMAKE_CURRENT_LIST_DIR}/../../..)

include(${CMAKE_CURRENT_LIST_DIR}/../test_deps.cmake)

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_CURRENT_LIST_DIR} src/*.c)
set(APP_C_SRCS ${SOURCES})
set(APP_INCLUDES src)

include(${CMAKE_CURRENT_LIST_DIR}/../shared/shared.cmake)

set(APP_COMPILER_FLAGS -Os
                       -DSMOKE_TEST=$<BOOL:${XMATH_SMOKE_TEST}>
                       -DUNITY_INCLUDE_CONFIG_H
                       )

if(NOT BUILD_NATIVE)
    list(APPEND APP_COMPILER_FLAGS -MMD
                                   -fxscope
                                   -mcmodel=large
                                   -Wno-xcore-fptrgroup
                                   -fcmdline-buffer-bytes=1024
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
