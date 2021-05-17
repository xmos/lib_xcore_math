
## Target name
set( LIB_NAME   unity )

## Path to Unity folder
set( UNITY_PATH ../../../third_party/Unity )

## Include directories to be exported
list( APPEND  INCLUDE_DIRS    ${UNITY_PATH}/src   )


## Compile flags for all platforms
unset(COMPILE_FLAGS)
list( APPEND  COMPILE_FLAGS         ""  )

## Platform-specific compile flags can go here
unset(COMPILE_FLAGS_XCORE)
list( APPEND  COMPILE_FLAGS_XCORE   -Wno-xcore-fptrgroup  )

unset(COMPILE_FLAGS_Linux)
list( APPEND  COMPILE_FLAGS_Linux   ""  )

###########

list( APPEND  COMPILE_FLAGS ${COMPILE_FLAGS_${CMAKE_SYSTEM_NAME}} )

## Library is a single source file
add_library( ${LIB_NAME} STATIC   ${UNITY_PATH}/src/unity.c )

target_include_directories( ${LIB_NAME} PUBLIC ${INCLUDE_DIRS} )

target_compile_options(${LIB_NAME} PRIVATE ${COMPILE_FLAGS} )

set_target_properties(${LIB_NAME} PROPERTIES  PREFIX ""
                                              OUTPUT_NAME ${LIB_NAME}
                                              SUFFIX ".a" )