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
        "${gmock_SOURCE_DIR}/include"
        "${gtest_SOURCE_DIR}/include"
    )
    target_link_libraries(${test_name} PRIVATE bstk gmock gtest gtest_main)
    add_test(NAME ${test_name} COMMAND ${test_name})
endfunction()

add_isolated_test(test_concurrency_lock_guard "src/concurrency/lock_guard/test.cpp")
add_isolated_test(test_concurrency_shared_lock "src/concurrency/shared_lock/test.cpp")
add_isolated_test(test_concurrency_shared_lock_guard "src/concurrency/shared_lock_guard/test.cpp")
add_isolated_test(test_concurrency_shared_resource "src/concurrency/shared_resource/test.cpp")
add_isolated_test(test_concurrency_sync_flag "src/concurrency/sync_flag/test.cpp")
add_isolated_test(test_core_bitmask "src/core/bitmask/test.cpp")
add_isolated_test(test_core_compressed_pair "src/core/compressed_pair/test.cpp")
add_isolated_test(test_core_exception "src/core/exception/test.cpp")
add_isolated_test(test_core_version "src/core/version/test.cpp")
add_isolated_test(test_memory_allocator "src/memory/allocator/test.cpp")
add_isolated_test(test_memory_debug_block "src/memory/debug_block/test.cpp")
add_isolated_test(test_memory_endian "src/memory/endian/test.cpp")
add_isolated_test(test_memory_memory_block "src/memory/memory_block/test.cpp")
add_isolated_test(test_memory_memory_block_view "src/memory/memory_block_view/test.cpp")
add_isolated_test(test_memory_object "src/memory/object/test.cpp")
add_isolated_test(test_memory_object_allocator "src/memory/object_allocator/test.cpp")
add_isolated_test(test_memory_reference_counter "src/memory/reference_counter/test.cpp")
add_isolated_test(test_memory_shared_array "src/memory/shared_array/test.cpp")
add_isolated_test(test_memory_shared_ptr "src/memory/shared_ptr/test.cpp")
add_isolated_test(test_memory_smart_pointer "src/memory/smart_pointer/test.cpp")
add_isolated_test(test_memory_sync_allocator "src/memory/sync_allocator/test.cpp")
add_isolated_test(test_memory_system_allocator "src/memory/system_allocator/test.cpp")
add_isolated_test(test_memory_unique_array "src/memory/unique_array/test.cpp")
add_isolated_test(test_memory_unique_ptr "src/memory/unique_ptr/test.cpp")
add_isolated_test(test_text_char_traits "src/text/char_traits/test.cpp")
add_isolated_test(test_text_encoding "src/text/encoding/test.cpp")
add_isolated_test(test_text_string "src/text/string/test.cpp")
add_isolated_test(test_text_string_const_iterator "src/text/string_const_iterator/test.cpp")
add_isolated_test(test_text_string_iterator "src/text/string_iterator/test.cpp")
add_isolated_test(test_text_string_view "src/text/string_view/test.cpp")
add_isolated_test(test_text_string_view_iterator "src/text/string_view_iterator/test.cpp")

# use a custom target to combine all targets into a single one,
# this allows only one post-build call instead of per-test copying
add_custom_target(bstk_and_tests ALL DEPENDS
    bstk
    test_concurrency_lock_guard
    test_concurrency_shared_lock
    test_concurrency_shared_lock_guard
    test_concurrency_shared_resource
    test_concurrency_sync_flag
    test_core_bitmask
    test_core_compressed_pair
    test_core_exception
    test_core_version
    test_memory_allocator
    test_memory_debug_block
    test_memory_endian
    test_memory_memory_block
    test_memory_memory_block_view
    test_memory_object
    test_memory_object_allocator
    test_memory_reference_counter
    test_memory_shared_array
    test_memory_shared_ptr
    test_memory_smart_pointer
    test_memory_sync_allocator
    test_memory_system_allocator
    test_memory_unique_array
    test_memory_unique_ptr
    test_text_char_traits
    test_text_encoding
    test_text_string
    test_text_string_const_iterator
    test_text_string_iterator
    test_text_string_view
    test_text_string_view_iterator
)
add_custom_command(TARGET bstk_and_tests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "$<TARGET_FILE:bstk>"
    "${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>" # copy to the tests' output directory
)