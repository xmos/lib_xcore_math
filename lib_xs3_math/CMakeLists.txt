
## Target name
set( LIB_NAME   lib_xs3_math  )

## Export lib directory as variable
set( XS3_MATH_PATH ${CMAKE_CURRENT_SOURCE_DIR} PARENT_SCOPE )

include(lib_xs3_math.cmake)

## Deliverable is a static library
add_library( ${LIB_NAME}  STATIC      ${LIB_XS3_MATH_SOURCES} )

target_include_directories( ${LIB_NAME} PUBLIC   ${LIB_XS3_MATH_INCLUDES}  )

set_target_properties( ${LIB_NAME} PROPERTIES   PREFIX      ""
                                                OUTPUT_NAME ${LIB_NAME}
                                                SUFFIX      ".a"        )

target_compile_options( ${LIB_NAME} PRIVATE ${COMPILE_FLAGS} )

target_compile_options( ${LIB_NAME} PRIVATE "$<$<CONFIG:DEBUG>:-DDEBUGG=1>" )


