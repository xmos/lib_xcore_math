cmake_minimum_required(VERSION 3.14)

## Set up the build options
include(etc/build_options.cmake)

find_package( Python3 COMPONENTS Interpreter )

project(xmos-lib_xcore_math)

## CMake configuration stuff
enable_language( C CXX ASM )
set( CMAKE_CXX_STANDARD           11  )
set( CMAKE_CXX_STANDARD_REQUIRED  ON  )
set( CMAKE_CXX_EXTENSIONS         OFF )

add_compile_options( ${COMPILE_FLAGS} )

## Add libs and apps
add_subdirectory( lib_xcore_math )
