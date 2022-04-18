## Source files
file( GLOB_RECURSE    LIB_XS3_MATH_C_SOURCES       "src/bfp/*.c" "src/vect/*.c" "src/scalar/*.c" )
file( GLOB_RECURSE    LIB_XS3_MATH_CPP_SOURCES     src/*.cpp )
file( GLOB_RECURSE    LIB_XS3_MATH_ASM_SOURCES     src/*.S   )

file( GLOB_RECURSE    LIB_XS3_MATH_C_SOURCES_REF "src/arch/ref/*.c" )

## Compile flags for all platforms
unset(LIB_XS3_MATH_COMPILE_FLAGS)
list( APPEND  LIB_XS3_MATH_COMPILE_FLAGS     -Wno-unused-variable -Wno-missing-braces)

## Platform-specific compile flags can go here
unset(LIB_XS3_MATH_COMPILE_FLAGS_XCORE_XS3A)
list( APPEND  LIB_XS3_MATH_COMPILE_FLAGS_XCORE_XS3A   -Wno-xcore-fptrgroup)

unset(LIB_XS3_MATH_COMPILE_FLAGS_Linux)
list( APPEND  LIB_XS3_MATH_COMPILE_FLAGS_Linux   ""  )

## set LIB_XS3_MATH_INCLUDES & LIB_XS3_MATH_SOURCES
set( LIB_XS3_MATH_INCLUDES     "${CMAKE_CURRENT_LIST_DIR}/api"           )

unset(LIB_XS3_MATH_SOURCES)
list( APPEND  LIB_XS3_MATH_SOURCES   ${LIB_XS3_MATH_C_SOURCES}    )
list( APPEND  LIB_XS3_MATH_SOURCES   ${LIB_XS3_MATH_CPP_SOURCES}  )

unset(LIB_XS3_MATH_SOURCES_XCORE_XS3A)
list( APPEND  LIB_XS3_MATH_SOURCES_XCORE_XS3A   ${LIB_XS3_MATH_ASM_SOURCES} )

unset(LIB_XS3_MATH_SOURCES_Linux) 
list( APPEND  LIB_XS3_MATH_SOURCES_Linux   ${LIB_XS3_MATH_C_SOURCES_REF} )
## Combine platform-agnostic and platform-specific variables..
list( APPEND  LIB_XS3_MATH_COMPILE_FLAGS    ${LIB_XS3_MATH_COMPILE_FLAGS_${CMAKE_SYSTEM_NAME}} )

list( APPEND  LIB_XS3_MATH_SOURCES  ${LIB_XS3_MATH_SOURCES_${CMAKE_SYSTEM_NAME}}        )

# Set defaults for generating the FFT LUT if they are not already defined
if ( NOT DEFINED GEN_FFT_LUT )
  set( GEN_FFT_LUT false )
endif()
if ( NOT DEFINED USE_DEFAULT_FFT_LUT )
  set( USE_DEFAULT_FFT_LUT true )
endif()

set( GEN_FFT_LUT  OFF )
if ( NOT ${GEN_FFT_LUT} )
  # Don't auto-generate the FFT LUT
  if( ${USE_DEFAULT_FFT_LUT} )
    # Use the FFT LUT stored in the repository
    set( XS3_FFT_LUT_FALLBACK_DIR ${CMAKE_CURRENT_LIST_DIR}/src/etc/xs3_fft_lut )
    set( FFT_LUT_FILE "xs3_fft_lut" )
    set( FFT_LUT_FILE_SOURCE  ${XS3_FFT_LUT_FALLBACK_DIR}/${FFT_LUT_FILE}.c )
    set( FFT_LUT_FILE_HEADER  ${XS3_FFT_LUT_FALLBACK_DIR}/${FFT_LUT_FILE}.h )

    list( APPEND LIB_XS3_MATH_SOURCES  ${FFT_LUT_FILE_SOURCE} )
    list( APPEND LIB_XS3_MATH_INCLUDES "${CMAKE_CURRENT_LIST_DIR}/src/etc/xs3_fft_lut" )
  else()
    # Otherwise, the user must use the provided python script to generate their own. See the Notes
    # section in the documentation for details.
  endif()
endif()   


## cmake doesn't recognize .S files as assembly by default
set_source_files_properties( ${LIB_XS3_MATH_ASM_SOURCES} PROPERTIES LANGUAGE ASM )

foreach(COMPILE_FLAG ${LIB_XS3_MATH_COMPILE_FLAGS})
  set_source_files_properties( ${LIB_XS3_MATH_SOURCES} PROPERTIES COMPILE_FLAGS ${COMPILE_FLAG}) 
endforeach()
