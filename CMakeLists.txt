cmake_minimum_required(VERSION 3.14)

# https://github.com/xmos/lib_xs3_math/issues/78
cmake_policy(SET CMP0057 NEW)

## Set up the build options
include(etc/build_options.cmake)

find_package( Python3 COMPONENTS Interpreter )

project(xmos-lib_xcore_math)

if( PROJECT_IS_TOP_LEVEL )
  message(STATUS "lib_xcore_math is top-level project.")
  set( DEV_LIB_XCORE_MATH ON )
endif()


# if( UNIX )
#   message(STATUS "Enabling Linter")
#   set(CMAKE_C_CLANG_TIDY "clang-tidy;-checks=*")
#   set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
# endif()


## CMake configuration stuff
enable_language( C CXX ASM )
set( CMAKE_CXX_STANDARD           11  )
set( CMAKE_CXX_STANDARD_REQUIRED  ON  )
set( CMAKE_CXX_EXTENSIONS         OFF )

add_compile_options( ${COMPILE_FLAGS} )

## Add libs and apps

add_subdirectory( lib_xcore_math )

if( ${DEV_LIB_XCORE_MATH} )
  message(STATUS "Including test and example build targets.")
  add_custom_target( lib_xcore_math-all )
  add_subdirectory( test )
  add_subdirectory( example )
endif()
