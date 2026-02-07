# CMakeLists.txt

# Copyright (c) Bitstro Systems. All rights reserved.
# Licensed under BSDL 1.0

cmake_minimum_required(VERSION 3.30.0)
project(bstk_library LANGUAGES CXX)

# check if the platform architecture is supported (BSTK_PLATFORM_ARCH defined in presets)
if(NOT ${BSTK_PLATFORM_ARCH} MATCHES "^x64$|^x86$")
    message(FATAL_ERROR "The BSTK library can only be built for x64 and x86 architectures.")
endif()

option(BSTK_BUILD_BENCHMARKS "Build BSTK library benchmarks" OFF)
option(BSTK_BUILD_TESTS "Build BSTK library tests" OFF)

# build the BSTK library before building any benchmarks and tests
add_subdirectory(src "${CMAKE_CURRENT_BINARY_DIR}/library")

if(BSTK_BUILD_BENCHMARKS)
    add_subdirectory(benchmarks "${CMAKE_CURRENT_BINARY_DIR}/benchmarks")
endif()

if(BSTK_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests "${CMAKE_CURRENT_BINARY_DIR}/tests")
endif()