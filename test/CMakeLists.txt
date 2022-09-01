
add_custom_target( lib_xcore_math-tests )


## Some of the test applications require the Unity test framework
include(FetchContent)
FetchContent_Declare(
  unity
  GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
  GIT_TAG        v2.5.2
  GIT_SHALLOW    TRUE
  SOURCE_DIR     unity
)

# Using FetchContent_MakeAvailable( unity ) will clone the Unity repo and 
# call add_subdirectory() on it, using the CMakeLists.txt in the repo.
# However, that didn't immediately work. Rather than investigate and fix that,
# I've just used a custom CMakeLists.txt
add_subdirectory( etc/unity )

## Some simple local libraries used by unit test apps
add_subdirectory( shared )

## The unit test apps

add_subdirectory( scalar_tests )
add_subdirectory( vect_tests )
add_subdirectory( bfp_tests  )
add_subdirectory( fft_tests  )
add_subdirectory( dct_tests  )
add_subdirectory( filter_tests  )
add_subdirectory( xs3_tests  )


add_dependencies( lib_xcore_math-all lib_xcore_math-tests )