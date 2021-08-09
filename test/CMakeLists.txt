## Just adds more subdirs to the cmake build.

## Some simple local libraries used by unit test apps
add_subdirectory( shared )

## The unit test apps

add_subdirectory( vect_tests )
add_subdirectory( bfp_tests  )
add_subdirectory( fft_tests  )
add_subdirectory( filter_tests  )