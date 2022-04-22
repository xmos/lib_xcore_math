
# https://github.com/xmos/lib_xs3_math/issues/78
cmake_policy(SET CMP0057 NEW)

## Set up the build options
include(etc/build_options.cmake)

find_package( Python3 COMPONENTS Interpreter )

## Compile flags for C/C++ for all libs and apps (for all platforms)
# [TODO] Move these into individual apps.
list(APPEND   COMPILE_FLAGS        -Os -g        )
list(APPEND   COMPILE_FLAGS        -Wno-format   )

if( ${DEV_LIB_XS3_MATH} )
  if( ${CMAKE_SYSTEM_NAME} STREQUAL "XCORE_XS3A" )
    list(APPEND COMPILE_FLAGS -MMD)
  endif()
endif()

## [TODO] Move these into individual apps.
## Platform-specific C/C++ compile flags
list(APPEND   COMPILE_FLAGS_Linux  -DUSE_REF=1   )

list(APPEND   COMPILE_FLAGS ${COMPILE_FLAGS_${CMAKE_SYSTEM_NAME}} )


## CMake configuration stuff
enable_language( C CXX ASM )
set( CMAKE_CXX_STANDARD           11  )
set( CMAKE_CXX_STANDARD_REQUIRED  ON  )
set( CMAKE_CXX_EXTENSIONS         OFF )

add_compile_options( ${COMPILE_FLAGS} )

## Add libs and apps

add_subdirectory( lib_xs3_math )

if( ${DEV_LIB_XS3_MATH} )
  add_custom_target( lib_xs3_math-all )
  add_subdirectory( test )
  add_subdirectory( example )
endif()
