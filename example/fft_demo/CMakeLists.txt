set( APP_NAME  fft_demo )

## If XCORE_TARGET hasn't been specified, default to XCORE-AI-EXPLORER
if( NOT DEFINED XCORE_TARGET )
  set( XCORE_TARGET XCORE-AI-EXPLORER )
endif()

add_executable( ${APP_NAME} )

target_sources( ${APP_NAME}
  PRIVATE
    main.c 
)

target_compile_options( ${APP_NAME}
  PRIVATE 
    -Os
    $<$<PLATFORM_ID:XCORE_XS3A>:-fxscope>
)

# Add options that are not valid with with MSVC compiler
if ( NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options( ${APP_NAME}
    PRIVATE
      -g
      -Wno-unused-variable
      -Wno-format
  )
endif()

target_link_options( ${APP_NAME}
  PRIVATE
    $<$<PLATFORM_ID:XCORE_XS3A>:-target=${XCORE_TARGET}>
    $<$<PLATFORM_ID:XCORE_XS3A>:-report>
    $<$<PLATFORM_ID:XCORE_XS3A>:-fxscope>
)

target_compile_definitions( ${APP_NAME}
  PRIVATE
    $<$<PLATFORM_ID:Linux>:USE_REF=1>
    $<$<PLATFORM_ID:Darwin>:USE_REF=1>
    $<$<PLATFORM_ID:Windows>:USE_REF=1>
)

target_link_libraries( ${APP_NAME}
  lib_xcore_math
    $<$<PLATFORM_ID:Linux>:m>
    $<$<PLATFORM_ID:Darwin>:m>
    #$<$<PLATFORM_ID:Windows>:m>
)

add_dependencies( lib_xcore_math-examples  ${APP_NAME} )