cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(xc_compat)

set(APP_HW_TARGET XK-EVK-XU316)

set(XMOS_SANDBOX_DIR ${CMAKE_CURRENT_LIST_DIR}/../../..)

set(APP_DEPENDENT_MODULES "lib_xcore_math")

XMOS_REGISTER_APP()
