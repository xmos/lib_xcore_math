

include(CMakeDependentOption)


#### BUILD OPTIONS ####

### Indicates that you're developing lib_xcore_math in this build
## If enabled, the unit tests and examples will also be built.
set( DEV_LIB_XCORE_MATH OFF CACHE BOOL "Include unit tests and examples as CMake Targets." )

## If enabled, the unit tests will be run fewer repetitions of each test to speed up execution time
option(XMATH_SMOKE_TEST "Build unit tests in 'smoke test' mode. This mostly just reduces the number of repetitions to reduce simulation time." OFF)

## If enabled, the FFT look-up tables will be auto-generated at build time (requires Python 3 and numpy)
set( XMATH_GEN_FFT_LUT  OFF CACHE BOOL "Auto-generate FFT look-up tables." )

## If enabled (and GEN_FFT_LUT disabled), use the FFT look-up tables provided with the repository.
## If both GEN_FFT_LUT and USE_DEFAULT_FFT_LUT are disabled, it is assumed that the user has manually
## generated the LUT using the provided python script and has added the appropriate sources and includes.
set( XMATH_USE_DEFAULT_FFT_LUT  ON CACHE BOOL "Use default provided FFT look-up table. (ignored if GEN_FFT_LUT is enabled)." )

## The maximum FFT length supported by the LUT (log2)
set( XMATH_MAX_FFT_LEN_LOG2 "10" CACHE STRING "Maximum FFT length to be supported by generated look-up tables. Must be a positive integer." )
