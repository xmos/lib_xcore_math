
## Target name
set( LIB_NAME   lib_xs3_math  )

## Export lib directory as variable
# [TODO] Looks like something might still use this.. why?
# set( XS3_MATH_PATH ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE )


## Source files
file( GLOB_RECURSE    SOURCES_C   "src/bfp/*.c" 
                                  "src/vect/*.c" 
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
)

target_include_directories( ${LIB_NAME}
  PUBLIC
    api
)

target_compile_options( ${LIB_NAME}
  PRIVATE
    -Os -g 
    -Wno-format 
    -Wno-unused-variable 
    -Wno-missing-braces
    $<$<PLATFORM_ID:XCORE_XS3A>:-Wno-xcore-fptrgroup>
)



# Set defaults for generating the FFT LUT if they are not already defined
if ( NOT DEFINED XS3_MATH_GEN_FFT_LUT )
  set( XS3_MATH_GEN_FFT_LUT false )
endif()

if ( NOT DEFINED XS3_MATH_USE_DEFAULT_FFT_LUT )
  set( XS3_MATH_USE_DEFAULT_FFT_LUT true )
endif()

if ( ${XS3_MATH_GEN_FFT_LUT} )
  # Auto-generate the FFT LUT

  if( NOT ${Python3_FOUND} )
    message(FATAL_ERROR "FFT LUT generation enabled, but Python3 not found. " 
                        "Disable the GEN_FFT_LUT option to use the version included with the repository (max FFT length=1024). "
                        "Disable the USE_DEFAULT_FFT_LUT option if you've manually generated the LUT.")
  endif()
  
  set( XS3_MATH_AUTOGEN_DIR  ${CMAKE_CURRENT_BINARY_DIR}/src.gen )

  file(MAKE_DIRECTORY ${XS3_MATH_AUTOGEN_DIR})

  set( FFT_LUT_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/script/gen_fft_table.py )
  set( FFT_LUT_FILE "xs3_fft_lut" )
  set( FFT_LUT_FILE_SOURCE  ${XS3_MATH_AUTOGEN_DIR}/${FFT_LUT_FILE}.c )
  set( FFT_LUT_FILE_HEADER  ${XS3_MATH_AUTOGEN_DIR}/${FFT_LUT_FILE}.h )

  list(APPEND FFT_SCRIPT_ARGS --out_file ${FFT_LUT_FILE})
  list(APPEND FFT_SCRIPT_ARGS --out_dir ${XS3_MATH_AUTOGEN_DIR})
  list(APPEND FFT_SCRIPT_ARGS --max_fft_log2 ${XS3_MATH_MAX_FFT_LEN_LOG2}) 
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
      ${XS3_MATH_AUTOGEN_DIR}
  )

else()
  # Don't auto-generate the FFT LUT
  if( ${XS3_MATH_USE_DEFAULT_FFT_LUT} )
    # Use the FFT LUT stored in the repository
    set( XS3_FFT_LUT_FALLBACK_DIR ${CMAKE_CURRENT_LIST_DIR}/src/etc/xs3_fft_lut )
    set( FFT_LUT_FILE "xs3_fft_lut" )
    set( FFT_LUT_FILE_SOURCE  ${XS3_FFT_LUT_FALLBACK_DIR}/${FFT_LUT_FILE}.c )
    set( FFT_LUT_FILE_HEADER  ${XS3_FFT_LUT_FALLBACK_DIR}/${FFT_LUT_FILE}.h )

    target_sources( ${LIB_NAME}
      PRIVATE
        ${FFT_LUT_FILE_SOURCE}
    )

    target_include_directories( ${LIB_NAME}
      PUBLIC
        ${XS3_FFT_LUT_FALLBACK_DIR}
    )

  else()
    # Otherwise, the user must use the provided python script to generate their own. See the Notes
    # section in the documentation for details.
  endif()
endif()   
