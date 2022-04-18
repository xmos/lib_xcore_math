## App name
set( APP_NAME  bfp_demo )

## If XCORE_TARGET hasn't been specified, default to XCORE-AI-EXPLORER
if( NOT DEFINED XCORE_TARGET )
  set( XCORE_TARGET XCORE-AI-EXPLORER )
endif()

## Depends on libraries
list( APPEND  DEP_LIBS        lib_xs3_math )

list( APPEND  DEP_LIBS_XCORE_XS3A  ""  )
list( APPEND  DEP_LIBS_Linux  m   )

## Sources
file( GLOB_RECURSE SOURCES_C  main.c )

# set( XSCOPE_CONFIG config.xscope )
get_filename_component(XSCOPE_CONFIG config.xscope ABSOLUTE)

## Compile flags
unset(COMPILE_FLAGS)
list( APPEND  COMPILE_FLAGS   -Wno-unused-variable )

unset(COMPILE_FLAGS_XCORE_XS3A)
list( APPEND  COMPILE_FLAGS_XCORE_XS3A    )

unset(COMPILE_FLAGS_Linux)
list( APPEND  COMPILE_FLAGS_XCORE_XS3A   "" )

## Linker flags
unset(LINKER_FLAGS)
list( APPEND  LINKER_FLAGS  "" )

unset(LINKER_FLAGS_XCORE_XS3A)
list( APPEND  LINKER_FLAGS_XCORE_XS3A  -target=${XCORE_TARGET} )
list( APPEND  LINKER_FLAGS_XCORE_XS3A  -report                 )
list( APPEND  LINKER_FLAGS_XCORE_XS3A  ${XSCOPE_CONFIG}        )

unset(LINKER_FLAGS_Linux)
list( APPEND  LINKER_FLAGS_Linux  ""                      )


#########

list( APPEND  SOURCES         ${SOURCES_C}     )
list( APPEND  SOURCES_XCORE_XS3A   ${SOURCES_XC}    )
list( APPEND  SOURCES   ${SOURCES_${CMAKE_SYSTEM_NAME}} )

list( APPEND  COMPILE_FLAGS ${COMPILE_FLAGS_${CMAKE_SYSTEM_NAME}} )

list( APPEND  LINKER_FLAGS ${LINKER_FLAGS_${CMAKE_SYSTEM_NAME}} )

list( APPEND  DEP_LIBS ${DEP_LIBS_${CMAKE_SYSTEM_NAME}} )

add_executable( ${APP_NAME} ${SOURCES} )

target_link_libraries( ${APP_NAME} ${DEP_LIBS})

target_compile_options( ${APP_NAME} PRIVATE ${COMPILE_FLAGS} )

#(because otherwise the set_target_properties command fails)
string(REPLACE ";" " " LINKER_FLAGS_STR "${LINKER_FLAGS}")


set_target_properties( ${APP_NAME} PROPERTIES LINK_FLAGS "${LINKER_FLAGS_STR}" )

if ( XCORE )
  set_target_properties( ${APP_NAME} PROPERTIES   SUFFIX ".xe" )
endif()
