
add_custom_target( lib_xcore_math-examples )

add_subdirectory( bfp_demo )
add_subdirectory( vect_demo )
add_subdirectory( fft_demo )
add_subdirectory( filter_demo )

add_dependencies( lib_xcore_math-all lib_xcore_math-examples )