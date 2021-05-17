cmake_minimum_required(VERSION 3.10)

project(xmos-lib_xs3_math)

## Compile flags for C/C++ for all libs and apps (for all platforms)
list(APPEND   COMPILE_FLAGS        -Os -g -MMD   )
list(APPEND   COMPILE_FLAGS        -Wno-format   )

## Platform-specific C/C++ compile flags
list(APPEND   COMPILE_FLAGS_XCORE  -march=xs3a   )
list(APPEND   COMPILE_FLAGS_Linux  -DUSE_REF=1   )

list(APPEND   COMPILE_FLAGS ${COMPILE_FLAGS_${CMAKE_SYSTEM_NAME}} )


## CMake configuration stuff

# list( APPEND  CMAKE_C_SOURCE_FILE_EXTENSIONS  xc )

enable_language( C CXX ASM )

set( CMAKE_CXX_STANDARD           11  )
set( CMAKE_CXX_STANDARD_REQUIRED  ON  )
set( CMAKE_CXX_EXTENSIONS         OFF )

add_compile_options( ${COMPILE_FLAGS} )

## Add libs and apps

add_subdirectory( lib_xs3_math )
add_subdirectory( test )