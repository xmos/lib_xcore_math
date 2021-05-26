
## Target name
set( LIB_NAME   lib_xs3_math  )

## Export lib directory as variable
set( XS3_MATH_PATH ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE )

## Library include directories to be exported
set( INCLUDE_DIRS     api           )

## Source files
file( GLOB_RECURSE    C_SOURCES       "src/bfp/*.c" "src/vect/*.c"  )
file( GLOB_RECURSE    CPP_SOURCES     src/*.cpp )
file( GLOB_RECURSE    ASM_SOURCES     src/*.S   )

file( GLOB_RECURSE    C_SOURCES_REF   "src/bfp/arch/ref/*.c" )


## Compile flags for all platforms
unset(COMPILE_FLAGS)
list( APPEND  COMPILE_FLAGS     -Wno-unused-variable
                                -Wno-missing-braces     )

## Platform-specific compile flags can go here
unset(COMPILE_FLAGS_XCORE)
list( APPEND  COMPILE_FLAGS_XCORE   -Wno-xcore-fptrgroup  )

unset(COMPILE_FLAGS_Linux)
list( APPEND  COMPILE_FLAGS_Linux   ""  )


########

## cmake doesn't recognize .S files as assembly by default
set_source_files_properties( ${ASM_SOURCES} PROPERTIES LANGUAGE ASM )

# Sources are the C, CPP and if building for xcore, ASM files             
unset(SOURCES)
list( APPEND  SOURCES   ${C_SOURCES}    )
list( APPEND  SOURCES   ${CPP_SOURCES}  )

unset(SOURCES_XCORE)
list( APPEND  SOURCES_XCORE   ${ASM_SOURCES}  )

unset(SOURCES_Linux)
list( APPEND  SOURCES_Linux   ${C_SOURCES_REF}  )

## Combine platform-agnostic and platform-specific variables..
list( APPEND  COMPILE_FLAGS  ${COMPILE_FLAGS_${CMAKE_SYSTEM_NAME}}  )
list( APPEND  SOURCES        ${SOURCES_${CMAKE_SYSTEM_NAME}}        )


## Deliverable is a static library
add_library( ${LIB_NAME}  STATIC      ${SOURCES} )

target_include_directories( ${LIB_NAME} PUBLIC   ${INCLUDE_DIRS}  )

set_target_properties( ${LIB_NAME} PROPERTIES   PREFIX      ""
                                                OUTPUT_NAME ${LIB_NAME}
                                                SUFFIX      ".a"        )

target_compile_options( ${LIB_NAME} PRIVATE ${COMPILE_FLAGS} )


