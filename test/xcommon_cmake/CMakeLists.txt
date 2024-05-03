cmake_minimum_required(VERSION 3.21)
include($ENV{XMOS_CMAKE_PATH}/xcommon.cmake)
project(xcommon_cmake_build)

set(APP_HW_TARGET XCORE-AI-EXPLORER)
set(APP_INCLUDES src)

set(APP_COMPILER_FLAGS -Os -Wall)

if(NOT BUILD_NATIVE)

list(APPEND APP_COMPILER_FLAGS -report)

endif()

# Add options for different compilers
if ( NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
list(APPEND APP_COMPILER_FLAGS
  -g
  -Werror
)
else()
list(APPEND APP_COMPILER_FLAGS
  # Suppress warning C4996: 'sprintf': This function or variable may be unsafe.
  # Consider using sprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
  # See online help for details.
  -D_CRT_SECURE_NO_WARNINGS
  # Suppress warning C5045: Compiler will insert Spectre mitigation for memory load if /wd5045 switch specified
  /wd5045
)
endif()

set(XMOS_SANDBOX_DIR ${CMAKE_CURRENT_LIST_DIR}/../../..)

set(APP_DEPENDENT_MODULES lib_xcore_math)


XMOS_REGISTER_APP()
