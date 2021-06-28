## App name
set( APP_NAME  filter_tests )

## Depends on libraries
list( APPEND  DEP_LIBS        lib_xs3_math 
                              unity 
                              test_shared )

list( APPEND  DEP_LIBS_XCORE  ""  )
list( APPEND  DEP_LIBS_Linux  m   )

## Sources
file( GLOB_RECURSE SOURCES_C  src/*.c )
file( GLOB_RECURSE SOURCES_XC src/*.xc )

# set( XSCOPE_CONFIG config.xscope )
get_filename_component(XSCOPE_CONFIG config.xscope ABSOLUTE)

## Compile flags
unset(COMPILE_FLAGS)
list( APPEND  COMPILE_FLAGS   -Wno-unused-variable )

unset(COMPILE_FLAGS_XCORE)
list( APPEND  COMPILE_FLAGS_XCORE    )

unset(COMPILE_FLAGS_Linux)
list( APPEND  COMPILE_FLAGS_XCORE   "" )

## Linker flags
unset(LINKER_FLAGS)
list( APPEND  LINKER_FLAGS  "" )

unset(LINKER_FLAGS_XCORE)
list( APPEND  LINKER_FLAGS_XCORE  -target=${XCORE_TARGET} )
list( APPEND  LINKER_FLAGS_XCORE  -report                 )
list( APPEND  LINKER_FLAGS_XCORE  ${XSCOPE_CONFIG}        )

unset(LINKER_FLAGS_Linux)
list( APPEND  LINKER_FLAGS_Linux  ""                      )

#########

# Run the python scripts for filter generation. This is just to ensure that the script runs
# correctly and produces code that compiles correctly.

list( APPEND SOURCES_C  ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local/test_fir_s32.c
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local/test_fir_s16.c
                        ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local/test_biquad_s32.c )

add_custom_target(
  generate_filter_fir_s32
  COMMAND python ${CMAKE_SOURCE_DIR}/lib_xs3_math/script/gen_fir_filter_s32.py test_fir_s32 --out-dir ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local ${CMAKE_CURRENT_SOURCE_DIR}/coefs.txt
  BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local/test_fir_s32.h ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local/test_fir_s32.c
  COMMENT "Generating test_fir_s32 from script" )

  
add_custom_target(
  generate_filter_fir_s16
  COMMAND python ${CMAKE_SOURCE_DIR}/lib_xs3_math/script/gen_fir_filter_s16.py test_fir_s16 --out-dir ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local ${CMAKE_CURRENT_SOURCE_DIR}/coefs.txt
  BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local/test_fir_s16.h ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local/test_fir_s16.c
  COMMENT "Generating test_fir_s16 from script" )

  
add_custom_target(
  generate_filter_biquad_s32
  COMMAND python ${CMAKE_SOURCE_DIR}/lib_xs3_math/script/gen_biquad_filter_s32.py test_biquad_s32 --out-dir ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local ${CMAKE_CURRENT_SOURCE_DIR}/biquad_coefs.csv
  BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local/test_biquad_s32.h ${CMAKE_CURRENT_SOURCE_DIR}/src/gen.local/test_biquad_s32.c
  COMMENT "Generating test_biquad_s32 from script" )

#########

list( APPEND  SOURCES         ${SOURCES_C}     )
list( APPEND  SOURCES_XCORE   ${SOURCES_XC}    )
list( APPEND  SOURCES   ${SOURCES_${CMAKE_SYSTEM_NAME}} )

list( APPEND  COMPILE_FLAGS ${COMPILE_FLAGS_${CMAKE_SYSTEM_NAME}} )

list( APPEND  LINKER_FLAGS ${LINKER_FLAGS_${CMAKE_SYSTEM_NAME}} )

list( APPEND  DEP_LIBS ${DEP_LIBS_${CMAKE_SYSTEM_NAME}} )

#########

add_executable( ${APP_NAME} ${SOURCES} )

target_link_libraries( ${APP_NAME} ${DEP_LIBS})

target_compile_options( ${APP_NAME} PRIVATE ${COMPILE_FLAGS} )

#(because otherwise the set_target_properties command fails)
string(REPLACE ";" " " LINKER_FLAGS_STR "${LINKER_FLAGS}")


set_target_properties( ${APP_NAME} PROPERTIES LINK_FLAGS "${LINKER_FLAGS_STR}" )

if ( XCORE )
  set_target_properties( ${APP_NAME} PROPERTIES   SUFFIX ".xe" )
endif()

add_dependencies( ${APP_NAME} generate_filter_fir_s32
                              generate_filter_fir_s16
                              generate_filter_biquad_s32 )