## Source files
file( GLOB_RECURSE    LIB_XS3_MATH_C_SOURCES       "src/bfp/*.c" "src/vect/*.c"   )
file( GLOB_RECURSE    LIB_XS3_MATH_CPP_SOURCES     src/*.cpp )
file( GLOB_RECURSE    LIB_XS3_MATH_ASM_SOURCES     src/*.S   )

file( GLOB_RECURSE    LIB_XS3_MATH_C_SOURCES_REF "src/arch/ref/*.c" )

## Compile flags for all platforms
unset(LIB_XS3_MATH_COMPILE_FLAGS)
list( APPEND  LIB_XS3_MATH_COMPILE_FLAGS     -Wno-unused-variable -Wno-missing-braces)

## Platform-specific compile flags can go here
unset(LIB_XS3_MATH_COMPILE_FLAGS_XCORE)
list( APPEND  LIB_XS3_MATH_COMPILE_FLAGS_XCORE   -Wno-xcore-fptrgroup)

unset(LIB_XS3_MATH_COMPILE_FLAGS_Linux)
list( APPEND  LIB_XS3_MATH_COMPILE_FLAGS_Linux   ""  )

## set LIB_XS3_MATH_INCLUDES & LIB_XS3_MATH_SOURCES
set( LIB_XS3_MATH_INCLUDES     api           )

unset(LIB_XS3_MATH_SOURCES)
list( APPEND  LIB_XS3_MATH_SOURCES   ${LIB_XS3_MATH_C_SOURCES}    )
list( APPEND  LIB_XS3_MATH_SOURCES   ${LIB_XS3_MATH_CPP_SOURCES}  )

unset(LIB_XS3_MATH_SOURCES_XCORE)
list( APPEND  LIB_XS3_MATH_SOURCES_XCORE   ${LIB_XS3_MATH_ASM_SOURCES} )

unset(LIB_XS3_MATH_SOURCES_Linux) 
list( APPEND  LIB_XS3_MATH_SOURCES_Linux   ${LIB_XS3_MATH_C_SOURCES_REF} )
## Combine platform-agnostic and platform-specific variables..
list( APPEND  LIB_XS3_MATH_COMPILE_FLAGS    ${LIB_XS3_MATH_COMPILE_FLAGS_${CMAKE_SYSTEM_NAME}} )

list( APPEND  LIB_XS3_MATH_SOURCES  ${LIB_XS3_MATH_SOURCES_${CMAKE_SYSTEM_NAME}}        )

## cmake doesn't recognize .S files as assembly by default
set_source_files_properties( ${LIB_XS3_MATH_ASM_SOURCES} PROPERTIES LANGUAGE ASM )

foreach(COMPILE_FLAG ${LIB_XS3_MATH_COMPILE_FLAGS})
  set_source_files_properties( ${LIB_XS3_MATH_SOURCES} PROPERTIES COMPILE_FLAGS ${COMPILE_FLAG}) 
endforeach()
