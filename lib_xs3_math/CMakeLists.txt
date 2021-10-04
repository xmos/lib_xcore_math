
## Target name
set( LIB_NAME   lib_xs3_math  )

## Export lib directory as variable
set( XS3_MATH_PATH ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE )

include(lib_xs3_math.cmake)


#### Generate FFT Look-up Tables ####

message(WARNING  "!!${CMAKE_CURRENT_BUILD_DIR}!!")
set( XS3_MATH_AUTOGEN_DIR  ${CMAKE_CURRENT_BUILD_DIR}/src.gen )


if ( ${GEN_FFT_LUT} )
  # Auto-generate the FFT LUT

  if( NOT ${Python3_FOUND} )
    message(FATAL_ERROR "FFT LUT generation enabled, but Python3 not found. " 
                        "Disable the GEN_FFT_LUT option to use the version included with the repository (max FFT length=1024). "
                        "Disable the USE_DEFAULT_FFT_LUT option if you've manually generated the LUT.")
  endif()

  file(MAKE_DIRECTORY ${XS3_MATH_AUTOGEN_DIR})

  set( FFT_LUT_SCRIPT ${CMAKE_SOURCE_DIR}/lib_xs3_math/script/gen_fft_table.py )
  set( FFT_LUT_FILE "xs3_fft_lut" )
  set( FFT_LUT_FILE_SOURCE  ${XS3_MATH_AUTOGEN_DIR}/${FFT_LUT_FILE}.c )
  set( FFT_LUT_FILE_HEADER  ${XS3_MATH_AUTOGEN_DIR}/${FFT_LUT_FILE}.h )
  set( FFT_LUT_BYPRODUCTS ${FFT_LUT_FILE_SOURCE} ${FFT_LUT_FILE_HEADER} )

  list(APPEND FFT_SCRIPT_ARGS --out_file ${FFT_LUT_FILE})
  list(APPEND FFT_SCRIPT_ARGS --out_dir ${XS3_MATH_AUTOGEN_DIR})
  list(APPEND FFT_SCRIPT_ARGS --max_fft_log2 ${MAX_FFT_LEN_LOG2})
  list(APPEND FFT_SCRIPT_ARGS --dit)
  list(APPEND FFT_SCRIPT_ARGS --dif)

  add_custom_command(
    OUTPUT ${FFT_LUT_BYPRODUCTS}
    COMMAND ${Python3_EXECUTABLE} ${FFT_LUT_SCRIPT} ${FFT_SCRIPT_ARGS}
    COMMENT "Generating FFT look-up tables" )

  list( APPEND LIB_XS3_MATH_SOURCES  ${FFT_LUT_FILE_SOURCE} )
  list( APPEND LIB_XS3_MATH_INCLUDES ${XS3_MATH_AUTOGEN_DIR} )
else()
  # Don't auto-generate the FFT LUT
  if( ${USE_DEFAULT_FFT_LUT} )
    # Use the FFT LUT stored in the repository
    set( XS3_FFT_LUT_FALLBACK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/etc/xs3_fft_lut )
    set( FFT_LUT_FILE "xs3_fft_lut" )
    set( FFT_LUT_FILE_SOURCE  ${XS3_FFT_LUT_FALLBACK_DIR}/${FFT_LUT_FILE}.c )
    set( FFT_LUT_FILE_HEADER  ${XS3_FFT_LUT_FALLBACK_DIR}/${FFT_LUT_FILE}.h )

    list( APPEND LIB_XS3_MATH_SOURCES  ${FFT_LUT_FILE_SOURCE} )
    list( APPEND LIB_XS3_MATH_INCLUDES ${XS3_FFT_LUT_FALLBACK_DIR} )
  else()
    # Otherwise, the user must use the provided python script to generate their own. See the Notes
    # section in the documentation for details.
  endif()
endif()   

#####################################

## Deliverable is a static library
add_library( ${LIB_NAME}  STATIC      ${LIB_XS3_MATH_SOURCES} )

target_include_directories( ${LIB_NAME} PUBLIC   ${LIB_XS3_MATH_INCLUDES}  )

set_target_properties( ${LIB_NAME} PROPERTIES   PREFIX      ""
                                                OUTPUT_NAME ${LIB_NAME}
                                                SUFFIX      ".a"        )

target_compile_options( ${LIB_NAME} PRIVATE ${COMPILE_FLAGS} )

target_compile_options( ${LIB_NAME} PRIVATE "$<$<CONFIG:DEBUG>:-DDEBUGG=1>" )

