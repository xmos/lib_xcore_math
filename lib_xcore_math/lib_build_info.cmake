set(LIB_NAME lib_xcore_math)

set(LIB_VERSION 2.1.4)

set(LIB_INCLUDES api
                src/etc/xmath_fft_lut # TODO put autogeneration
                src/vect)

set(LIB_COMPILER_FLAGS -Os
                       -Wall)

set(LIB_DEPENDENT_MODULES "")
set(LIB_XMATH_PATH ${XMOS_SANDBOX_DIR}lib_xcore_math/lib_xcore_math)

file( GLOB_RECURSE SOURCES_C RELATIVE ${LIB_XMATH_PATH}
                                  "${LIB_XMATH_PATH}/src/etc/xmath_fft_lut/*.c" # TODO put autogeneration
                                  "${LIB_XMATH_PATH}/src/vect/*.c"
                                  "${LIB_XMATH_PATH}/src/bfp/*.c"
                                  "${LIB_XMATH_PATH}/src/dct/*.c"
                                  "${LIB_XMATH_PATH}/src/fft/*.c"
                                  "${LIB_XMATH_PATH}/src/filter/*.c"
                                  "${LIB_XMATH_PATH}/src/scalar/*.c")


# Platform specific things
if(APP_BUILD_ARCH STREQUAL "xs3a")

  set(SOURCES_REF "")

else() # native or non-xs3a

  file( GLOB_RECURSE SOURCES_REF RELATIVE ${LIB_XMATH_PATH} 
                                  "${LIB_XMATH_PATH}/src/arch/ref/*.c" )
  set(LIB_ASM_SRCS "")

  if(BUILD_NATIVE AND (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC"))
    foreach(APP_TARGET ${APP_BUILD_TARGETS})
      target_link_libraries(${APP_TARGET} PRIVATE m)
    endforeach()
  endif()
endif()

# Add options for different compilers
if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
list(APPEND LIB_COMPILER_FLAGS
  -g
  -Wextra
  -Werror
)
else()
list(APPEND LIB_COMPILER_FLAGS
  -WX
  # Suppress warning C4996: 'sprintf': This function or variable may be unsafe.
  # Consider using sprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
  # See online help for details.
  -D_CRT_SECURE_NO_WARNINGS
  # Suppress warning C5045: Compiler will insert Spectre mitigation for memory load if /wd5045 switch specified
  /wd5045
)
endif()

set(LIB_C_SRCS ${SOURCES_C} ${SOURCES_REF})

XMOS_REGISTER_MODULE()
