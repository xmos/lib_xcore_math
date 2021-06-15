cmake_minimum_required(VERSION 3.10)

## Disable in-source build.
if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "In-source build is not allowed! Please specify a build folder.\n\tex:cmake -B build")
endif()

project(xmos-lib_xs3_math)

## fetch dependencies
include(FetchContent)

FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG        v2.5.2
  GIT_SHALLOW    TRUE
  SOURCE_DIR     unity
)
FetchContent_Populate(unity)
include(test/etc/Unity/CMakeLists.txt)

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
add_subdirectory( example )