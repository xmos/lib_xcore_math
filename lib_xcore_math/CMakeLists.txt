
## Target name
set( LIB_NAME   lib_xcore_math  )

## Export lib directory as variable
# [TODO] Looks like something might still use this.. why?
# set( XMATH_MATH_PATH ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE )


## Source files
file( GLOB_RECURSE    SOURCES_C   "src/vect/*.c"
                                  "src/bfp/*.c"
                                  "src/dct/*.c"
                                  "src/fft/*.c"
                                  "src/filter/*.c"
                                  "src/scalar/*.c" )
file( GLOB_RECURSE    SOURCES_CPP "src/*.cpp" )
file( GLOB_RECURSE    SOURCES_ASM "src/*.S"   )
file( GLOB_RECURSE    SOURCES_REF "src/arch/ref/*.c" )

add_library( ${LIB_NAME}  STATIC )

target_sources( ${LIB_NAME}
  PRIVATE
    ${SOURCES_C}
    ${SOURCES_CPP}
    $<$<PLATFORM_ID:XCORE_XS3A>:${SOURCES_ASM}>
    $<$<PLATFORM_ID:Linux>:${SOURCES_REF}>
    $<$<PLATFORM_ID:Darwin>:${SOURCES_REF}>
    $<$<PLATFORM_ID:Windows>:${SOURCES_REF}>
)

target_include_directories( ${LIB_NAME}
  PUBLIC
    api
)

target_include_directories( ${LIB_NAME}
  PRIVATE
    src/vect
)

target_compile_options( ${LIB_NAME}
  PRIVATE
    -Os
    -Wall
    $<$<PLATFORM_ID:XCORE_XS3A>:-Wno-xcore-fptrgroup>
)

# Add options for different compilers
if ( NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(${LIB_NAME}
    PRIVATE
      -g
      -Wextra
      -Werror
  )
else()
  target_compile_options(${LIB_NAME}
    PRIVATE
      -WX
      # Suppress warning C4996: 'sprintf': This function or variable may be unsafe.
      # Consider using sprintf_s instead. To disable deprecation, use _CRT_SECURE_NO_WARNINGS.
      # See online help for details.
      -D_CRT_SECURE_NO_WARNINGS
      # Suppress warning C5045: Compiler will insert Spectre mitigation for memory load if /wd5045 switch specified
      /wd5045
)
endif()

# Set defaults for generating the FFT LUT if they are not already defined
if ( NOT DEFINED XMATH_GEN_FFT_LUT )
  set( XMATH_GEN_FFT_LUT OFF )
endif()

if ( NOT DEFINED XMATH_USE_DEFAULT_FFT_LUT )
  set( XMATH_USE_DEFAULT_FFT_LUT ON )
endif()

if ( ${XMATH_GEN_FFT_LUT} )
  # Auto-generate the FFT LUT

  if( NOT ${Python3_FOUND} )
    message(FATAL_ERROR "FFT LUT generation enabled, but Python3 not found. "
                        "Disable the XMATH_GEN_FFT_LUT option to use the version included with the repository (max FFT length=1024). "
                        "Disable the XMATH_USE_DEFAULT_FFT_LUT option if you've manually generated the LUT.")
  endif()
  
  set( XMATH_AUTOGEN_DIR  ${CMAKE_CURRENT_BINARY_DIR}/src.gen )

  file(MAKE_DIRECTORY ${XMATH_AUTOGEN_DIR})

  set( FFT_LUT_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/script/gen_fft_table.py )
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

  target_sources( ${LIB_NAME}
    PRIVATE
      ${FFT_LUT_FILE_SOURCE}
  )

  target_include_directories( ${LIB_NAME}
    PUBLIC
      ${XMATH_AUTOGEN_DIR}
  )

else()
  # Don't auto-generate the FFT LUT
  if( ${XMATH_USE_DEFAULT_FFT_LUT} )
    # Use the FFT LUT stored in the repository
    set( XMATH_FFT_LUT_FALLBACK_DIR ${CMAKE_CURRENT_LIST_DIR}/src/etc/xmath_fft_lut )
    set( FFT_LUT_FILE "xmath_fft_lut" )
    set( FFT_LUT_FILE_SOURCE  ${XMATH_FFT_LUT_FALLBACK_DIR}/${FFT_LUT_FILE}.c )
    set( FFT_LUT_FILE_HEADER  ${XMATH_FFT_LUT_FALLBACK_DIR}/${FFT_LUT_FILE}.h )

    target_sources( ${LIB_NAME}
      PRIVATE
        ${FFT_LUT_FILE_SOURCE}
    )

    target_include_directories( ${LIB_NAME}
      PUBLIC
        ${XMATH_FFT_LUT_FALLBACK_DIR}
    )

  else()
    # Otherwise, the user must use the provided python script to generate their own. See the Notes
    # section in the documentation for details.
  endif()
endif()
