set( LIB_NAME test_shared )

file( GLOB_RECURSE  SHARED_SOURCES_C   *.c  )
file( GLOB_RECURSE  SHARED_SOURCES_XC  *.xc )

list( APPEND  INCLUDE_DIRS  floating_fft
                            pseudo_rand
                            testing       )

###########


## Add library called "test_shared"
add_library( ${LIB_NAME} ${SHARED_SOURCES} )

target_sources( ${LIB_NAME}
  PUBLIC 
    ${SHARED_SOURCES_C}
    $<$<PLATFORM_ID:XCORE_XS3A>:${SHARED_SOURCES_XC}>
)

target_include_directories( ${LIB_NAME} 
  PUBLIC ${INCLUDE_DIRS} 
)

target_link_libraries( ${LIB_NAME} 
  lib_xcore_math 
)
