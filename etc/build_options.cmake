

include(CMakeDependentOption)



#### BUILD OPTIONS ####  

## If enabled, the unit tests will be added as build targets
set(BUILD_TESTS    ON CACHE BOOL "Include unit tests as CMake targets." )

## If enabled, the examples will be added as build targets
set(BUILD_EXAMPLES ON CACHE BOOL "Include examples as CMake targets."   )

## If enabled, the unit tests will be run fewer repetitions of each test to speed up execution time
option(SMOKE_TEST "Build unit tests in 'smoke test' mode. This mostly just reduces the number of repetitions to reduce simulation time." OFF)

## If enabled, the FFT look-up tables will be auto-generated at build time (requires Python 3 and numpy)
set( GEN_FFT_LUT  ON CACHE BOOL "Auto-generate FFT look-up tables." )

## If enabled (and GEN_FFT_LUT disabled), use the FFT look-up tables provided with the repository.
## If both GEN_FFT_LUT and USE_DEFAULT_FFT_LUT are disabled, it is assumed that the user has manually
## generated the LUT using the provided python script and has added the appropriate sources and includes.
set( USE_DEFAULT_FFT_LUT  ON CACHE BOOL "Use default provided FFT look-up table. (ignored if GEN_FFT_LUT is enabled)." )

## The maximum FFT length supported by the LUT (log2)
set( MAX_FFT_LEN_LOG2 "10" CACHE STRING "Maximum FFT length to be supported by generated look-up tables. Must be a positive integer." )

## Generate decimation-in-time FFT LUTs   (only enabled if GEN_FFT_LUT is ON)
cmake_dependent_option( GEN_FFT_LUT_DIT "Generate decimation-in-time FFT look-up table." ON "GEN_FFT_LUT" OFF ) 

## Generate decimation-in-frequency FFT LUTs   (only enabled if GEN_FFT_LUT is ON)
cmake_dependent_option( GEN_FFT_LUT_DIF "Generate decimation-in-frequency FFT look-up table." ON "GEN_FFT_LUT" OFF ) 


#### PRINT OPTIONS ####

message(STATUS "BUILD_TESTS:    ${BUILD_TESTS}")
message(STATUS "BUILD_EXAMPLES: ${BUILD_EXAMPLES}")
message(STATUS "SMOKE_TEST:     ${SMOKE_TEST}")

message(STATUS "GEN_FFT_LUT:    ${GEN_FFT_LUT}")
if(${GEN_FFT_LUT})
  message(STATUS "  MAX_FFT_LEN_LOG2:   ${MAX_FFT_LEN_LOG2}")
  message(STATUS "  GEN_FFT_LUT_DIT:    ${GEN_FFT_LUT_DIF}")
  message(STATUS "  GEN_FFT_LUT_DIF:    ${GEN_FFT_LUT_DIF}")
else()
  message(STATUS "USE_DEFAULT_FFT_LUT:    ${USE_DEFAULT_FFT_LUT}")
endif()
