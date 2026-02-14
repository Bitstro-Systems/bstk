# CMakeLists.txt

# Copyright (c) Bitstro Systems. All rights reserved.
# Licensed under BSDL 1.0

cmake_minimum_required(VERSION 3.30.0)
project(bstk_tests LANGUAGES CXX)

# build Google Test as a static library before building any tests
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(google-test "${CMAKE_CURRENT_BINARY_DIR}/google-test" EXCLUDE_FROM_ALL)

function(add_isolated_test test_name test_path)
    # create an executable for the specified test, linking against necessary libraries
    add_executable(${test_name} ${test_path})
    target_compile_features(${test_name} PRIVATE cxx_std_20)
    target_include_directories(${test_name} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../src"
        "${gtest_SOURCE_DIR}/include"
    )
    target_link_libraries(${test_name} PRIVATE bstk gtest gtest_main)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_isolated_test(test_core_exception "src/core/exception/test.cpp")
add_isolated_test(test_core_version "src/core/version/test.cpp")

# use a custom target to combine all targets into a single one,
# this allows only one post-build call instead of per-test copying
add_custom_target(bstk_and_tests ALL DEPENDS
    bstk
    test_core_exception
    test_core_version
)
add_custom_command(TARGET bstk_and_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:bstk>"
    "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>" # copy to the tests' output directory
)