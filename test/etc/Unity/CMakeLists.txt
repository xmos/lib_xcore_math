
## Target name
set( LIB_NAME   unity )

## Path to Unity folder
set( UNITY_PATH unity )

## Include directories to be exported
list( APPEND  INCLUDE_DIRS    
    ${CMAKE_CURRENT_BINARY_DIR}/${UNITY_PATH}/src
    ${CMAKE_CURRENT_BINARY_DIR}/${UNITY_PATH}/extras/fixture/src/
    ${CMAKE_CURRENT_BINARY_DIR}/${UNITY_PATH}/extras/memory/src/
)


## Compile flags for all platforms
unset(COMPILE_FLAGS)
list( APPEND  COMPILE_FLAGS         ""  )

## Platform-specific compile flags can go here
unset(COMPILE_FLAGS_XCORE)
list( APPEND  COMPILE_FLAGS_XCORE  -Wno-xcore-fptrgroup  )
list( APPEND  COMPILE_FLAGS_XCORE  -march=xs3a   )


unset(COMPILE_FLAGS_Linux)
list( APPEND  COMPILE_FLAGS_Linux   ""  )

###########

list( APPEND  COMPILE_FLAGS ${COMPILE_FLAGS_${CMAKE_SYSTEM_NAME}} )

## Library is a (almost) single source file
set(UNITY_SOURCES
    "${UNITY_PATH}/src/unity.c"
    "${UNITY_PATH}/extras/fixture/src/unity_fixture.c"
    "${UNITY_PATH}/extras/memory/src/unity_memory.c"
)
add_library( ${LIB_NAME} STATIC   ${UNITY_SOURCES}  )

target_include_directories( ${LIB_NAME} PUBLIC ${INCLUDE_DIRS} )

target_compile_options(${LIB_NAME} PRIVATE ${COMPILE_FLAGS} )

set_target_properties(${LIB_NAME} PROPERTIES  PREFIX ""
                                              OUTPUT_NAME ${LIB_NAME}
                                              SUFFIX ".a" )