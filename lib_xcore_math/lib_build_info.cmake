set(LIB_NAME lib_xcore_math)

set(LIB_VERSION 2.1.4)

set(LIB_INCLUDES api
                src/etc/xmath_fft_lut # TODO put autogeneration
                src/vect)

set(LIB_COMPILER_FLAGS -Os
                       -g
                       -Wall
                       -Wextra
                       -Werror)

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

if(BUILD_NATIVE)

file( GLOB_RECURSE SOURCES_REF RELATIVE ${LIB_XMATH_PATH} 
                                  "${LIB_XMATH_PATH}/src/arch/ref/*.c" )
set(LIB_ASM_SRCS "")

else()

set(SOURCES_REF "")

endif()

set(LIB_C_SRCS ${SOURCES_C} ${SOURCES_REF})

XMOS_REGISTER_MODULE()
