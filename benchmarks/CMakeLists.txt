# CMakeLists.txt

# Copyright (c) Bitstro Systems. All rights reserved.
# Licensed under BSDL 1.0

cmake_minimum_required(VERSION 3.30.0)
project(bstk_benchmarks LANGUAGES CXX)

# build Google Benchmark before building any benchmarks
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
add_subdirectory(google-benchmark "${CMAKE_CURRENT_BINARY_DIR}/google-benchmark" EXCLUDE_FROM_ALL)

function(add_isolated_benchmark benchmark_name benchmark_path)
    # create an executable for the specified benchmark, linking against necessary libraries
    add_executable(${benchmark_name} ${benchmark_path})
    target_compile_definitions(${benchmark_name} PRIVATE BENCHMARK_STATIC_DEFINE)
    target_compile_features(${benchmark_name} PRIVATE cxx_std_20)
    target_include_directories(${benchmark_name} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../src"
        "${benchmark_SOURCE_DIR}/include"
    )
    target_link_libraries(${benchmark_name} PRIVATE benchmark bstk)
    if(WIN32)
        # link against Shlwapi.lib, a Windows-specific statically-linked library
        target_link_libraries(${benchmark_name} PRIVATE Shlwapi)
    endif()
endfunction()

# Currently, there are no compatible benchmarks. However, the main build component
# has been implemented. Once sufficient progress is made, benchmarks will be added here.

# use a custom target to combine all targets into a single one,
# this allows only one post-build call instead of per-benchmark copying
add_custom_target(bstk_and_benchmarks ALL DEPENDS
    bstk
)
add_custom_command(TARGET bstk_and_benchmarks POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:bstk>"
    "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>" # copy to the benchmarks' output directory
)