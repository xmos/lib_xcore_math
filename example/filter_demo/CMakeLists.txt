set( APP_NAME  filter_demo )

## If XCORE_TARGET hasn't been specified, default to XCORE-AI-EXPLORER
if( NOT DEFINED XCORE_TARGET )
  set( XCORE_TARGET XCORE-AI-EXPLORER )
endif()

add_executable( ${APP_NAME} )

target_sources( ${APP_NAME}
  PRIVATE
    main.c
    filter_32bit_fir.c
    filter_16bit_fir.c
    filter_32bit_biquad.c
)

target_compile_options( ${APP_NAME}
  PRIVATE
    -Os
    $<$<PLATFORM_ID:XCORE_XS3A>:-fxscope>
)

# Add options for different compilers
if ( NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options( ${APP_NAME}
    PRIVATE
      -Werror
      -g
  )
else()
    target_compile_options( ${APP_NAME}
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
)

add_dependencies( lib_xcore_math-examples  ${APP_NAME} )