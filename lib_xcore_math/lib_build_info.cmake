set(LIB_NAME lib_xcore_math)

set(LIB_XMATH_PATH ${XMOS_SANDBOX_DIR}/lib_xcore_math/lib_xcore_math)
include(${XMOS_SANDBOX_DIR}/lib_xcore_math/lib_xcore_math/build_options.cmake)

set(LIB_VERSION 2.4.0)

set(LIB_INCLUDES api
                src/vect)

set(LIB_COMPILER_FLAGS -Os
                       -Wall)

set(LIB_DEPENDENT_MODULES "")

file( GLOB_RECURSE SOURCES_C RELATIVE ${LIB_XMATH_PATH}
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

  # need to link m on x86 non-windows platforms
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
    # Suppress warning C4996: 'sprintf': This function or variable may be unsafe.
    # Consider using sprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
    # See online help for details.
    -D_CRT_SECURE_NO_WARNINGS
    # Suppress warning C5045: Compiler will insert Spectre mitigation for memory load if /wd5045 switch specified
    /wd5045
  )
endif()

set(LIB_C_SRCS ${SOURCES_C} ${SOURCES_REF})

# Set defaults for generating the FFT LUT if they are not already defined
if ( NOT DEFINED XMATH_GEN_FFT_LUT )
  set( XMATH_GEN_FFT_LUT OFF )
endif()

if ( NOT DEFINED XMATH_USE_DEFAULT_FFT_LUT )
  set( XMATH_USE_DEFAULT_FFT_LUT ON )
endif()

if ( ${XMATH_GEN_FFT_LUT} )
  # Auto-generate the FFT LUT
  find_package(Python3 COMPONENTS Interpreter)

  if( NOT ${Python3_FOUND} )
    message(FATAL_ERROR "FFT LUT generation enabled, but Python3 not found. "
                        "Disable the XMATH_GEN_FFT_LUT option to use the version included with the repository (max FFT length=1024). "
                        "Disable the XMATH_USE_DEFAULT_FFT_LUT option if you've manually generated the LUT.")
  endif()

  set( XMATH_AUTOGEN_DIR ${CMAKE_CURRENT_BINARY_DIR}/src.gen )

  file(MAKE_DIRECTORY ${XMATH_AUTOGEN_DIR})

  set( FFT_LUT_SCRIPT ${LIB_XMATH_PATH}/python/gen_fft_table.py )
  set( FFT_LUT_FILE "xmath_fft_lut" )
  set( FFT_LUT_FILE_SOURCE  ${XMATH_AUTOGEN_DIR}/${FFT_LUT_FILE}.c )
  set( FFT_LUT_FILE_HEADER  ${XMATH_AUTOGEN_DIR}/${FFT_LUT_FILE}.h )

  list(APPEND FFT_SCRIPT_ARGS --out_file ${FFT_LUT_FILE})
  list(APPEND FFT_SCRIPT_ARGS --out_dir ${XMATH_AUTOGEN_DIR})
  list(APPEND FFT_SCRIPT_ARGS --max_fft_log2 ${XMATH_MAX_FFT_LEN_LOG2})
  list(APPEND FFT_SCRIPT_ARGS --dit)
  list(APPEND FFT_SCRIPT_ARGS --dif)

  add_custom_command(
    OUTPUT ${FFT_LUT_FILE_SOURCE} ${FFT_LUT_FILE_HEADER}
    COMMAND ${Python3_EXECUTABLE} ${FFT_LUT_SCRIPT} ${FFT_SCRIPT_ARGS}
    COMMENT "Generating FFT look-up tables" )

  foreach(APP_TARGET ${APP_BUILD_TARGETS})
    target_sources( ${APP_TARGET}
      PRIVATE
        ${FFT_LUT_FILE_SOURCE}
    )

    target_include_directories( ${APP_TARGET}
      PUBLIC
      ${XMATH_AUTOGEN_DIR}
    )
  endforeach()
# Don't auto-generate the FFT LUT
elseif(${XMATH_USE_DEFAULT_FFT_LUT})
  # Use the FFT LUT stored in the repository
  set( XMATH_FFT_LUT_FALLBACK_DIR src/etc/xmath_fft_lut )
  set( FFT_LUT_FILE "xmath_fft_lut" )
  set( FFT_LUT_FILE_SOURCE  ${XMATH_FFT_LUT_FALLBACK_DIR}/${FFT_LUT_FILE}.c )
  set( FFT_LUT_FILE_HEADER  ${XMATH_FFT_LUT_FALLBACK_DIR}/${FFT_LUT_FILE}.h )


  list(APPEND LIB_C_SRCS ${FFT_LUT_FILE_SOURCE})
  list(APPEND LIB_INCLUDES ${XMATH_FFT_LUT_FALLBACK_DIR})

else()
  # Otherwise, the user must use the provided python script to generate their own. See the Notes
  # section in the documentation for details.
endif()

XMOS_REGISTER_MODULE()
