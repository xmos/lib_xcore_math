set( APP_NAME  bfp_tests )

## If XCORE_TARGET hasn't been specified, default to XCORE-AI-EXPLORER
if( NOT DEFINED XCORE_TARGET )
  set( XCORE_TARGET XCORE-AI-EXPLORER )
endif()


## Sources
file( GLOB_RECURSE SOURCES_C  src/*.c )
file( GLOB_RECURSE SOURCES_XC src/*.xc )

add_executable( ${APP_NAME} )

target_sources( ${APP_NAME}
  PRIVATE
    ${SOURCES_C}
    $<$<PLATFORM_ID:XCORE_XS3A>:${SOURCES_XC}>
)

target_include_directories( ${APP_NAME} 
  PRIVATE 
    src 
)

target_compile_options( ${APP_NAME}
  PRIVATE 
    -Os -g -Wno-format -Wno-unused-variable
    $<$<PLATFORM_ID:XCORE_XS3A>:-MMD>
    $<$<PLATFORM_ID:XCORE_XS3A>:-fxscope>
    $<$<PLATFORM_ID:XCORE_XS3A>:-mcmodel=large>
    $<$<PLATFORM_ID:XCORE_XS3A>:-Wno-xcore-fptrgroup>
)

target_link_options( ${APP_NAME}
  PRIVATE
    $<$<PLATFORM_ID:XCORE_XS3A>:-target=${XCORE_TARGET}>
    $<$<PLATFORM_ID:XCORE_XS3A>:-report>
    $<$<PLATFORM_ID:XCORE_XS3A>:-fcmdline-buffer-bytes=1024>
    $<$<PLATFORM_ID:XCORE_XS3A>:-fxscope>
)

target_compile_definitions( ${APP_NAME}
  PRIVATE
    $<$<PLATFORM_ID:Linux>:USE_REF=1>
    SMOKE_TEST=$<BOOL:${XS3_MATH_SMOKE_TEST}>
)

target_link_libraries( ${APP_NAME}
  lib_xs3_math
  test_shared
  unity
  $<$<PLATFORM_ID:Linux>:m>
)

add_dependencies( lib_xs3_math-tests  ${APP_NAME} )
