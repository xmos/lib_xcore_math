cmake_minimum_required(VERSION 3.14)

project(legacy_cmake_build)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../.. ${CMAKE_BINARY_DIR}/lib_xcore_math)

add_executable(legacy_cmake_build src/main.c)
target_link_libraries(legacy_cmake_build PRIVATE lib_xcore_math)

if(NOT BUILD_NATIVE)
    target_link_options(legacy_cmake_build PRIVATE -target=XK-EVK-XU316)
endif()
