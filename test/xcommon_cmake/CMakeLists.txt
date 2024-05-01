cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(xcommon_cmake_build)

set(APP_HW_TARGET XCORE-AI-EXPLORER)
set(APP_INCLUDES src)

set(APP_COMPILER_FLAGS
  -Os
  -g
  -Wall
  -Werror
  -mcmodel=large)

if(NOT BUILD_NATIVE)

set(APP_COMPILER_FLAGS ${APP_COMPILER_FLAGS} -report -fxscope)

endif()

set(XMOS_SANDBOX_DIR ${CMAKE_CURRENT_LIST_DIR}/../../..)

set(APP_DEPENDENT_MODULES lib_xcore_math)


XMOS_REGISTER_APP()

#if(BUILD_NATIVE)

#target_link_libraries(${PROJECT_NAME} PRIVATE m)

#endif()
