
## Defines for tests

unset(TEST_DEFS)
if( ${SMOKE_TEST} )
  list(APPEND   TEST_DEFS      -DSMOKE_TEST=1  )
else()
  list(APPEND   TEST_DEFS      -DSMOKE_TEST=0  )
endif()


## Some simple local libraries used by unit test apps
add_subdirectory( shared )

## The unit test apps

add_subdirectory( scalar_tests )
add_subdirectory( vect_tests )
add_subdirectory( bfp_tests  )
add_subdirectory( fft_tests  )
add_subdirectory( filter_tests  )