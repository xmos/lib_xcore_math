option(XMATH_SMOKE_TEST "Build unit tests in 'smoke test' mode. This mostly just reduces the number of repetitions to reduce simulation time." OFF)

set(SHARED_INCLUDES floating_fft pseudo_rand testing)
list(TRANSFORM SHARED_INCLUDES PREPEND ${CMAKE_CURRENT_LIST_DIR}/)
list(APPEND APP_INCLUDES ${SHARED_INCLUDES})

file(GLOB_RECURSE SHARED_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_LIST_DIR}/**/*.c)
list(APPEND APP_C_SRCS ${SHARED_SOURCES})
