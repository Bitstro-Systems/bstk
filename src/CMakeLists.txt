# CMakeLists.txt

# Copyright (c) Bitstro Systems. All rights reserved.
# Licensed under BSDL 1.0

cmake_minimum_required(VERSION 3.30.0)

set(BSTK_CORE_INC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/bstk/core/exception.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bstk/core/export.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bstk/core/version.hpp"
)
set(BSTK_CORE_SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/bstk/core/exception.cpp"
)
set(BSTK_CORE_IMPL_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/bstk/core/impl/dllmain.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bstk/core/impl/tinywin.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/bstk/core/impl/utils.hpp"
)
set(BSTK_RES_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/bstk/res/bstk.rc"
)

# internal BSTK library compile definitions
set(BSTK_INTERNAL_DEFS)

# detect the platform and set the corresponding definition, terminate if the platform is not supported
if(WIN32)
    list(APPEND BSTK_INTERNAL_DEFS _BSTK_WINDOWS=1)
elseif(UNIX)
    list(APPEND BSTK_INTERNAL_DEFS _BSTK_LINUX=1)
else()
    # unsupported platform, report an error and terminate
    message(FATAL_ERROR "Unsupported platform detected.")
endif()

# detect the compiler and set the corresponding definition, terminate if the compiler is not supported
set(is_gcc FALSE)
set(is_msvc FALSE)
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(is_gcc TRUE)
    list(APPEND BSTK_INTERNAL_DEFS _BSTK_GCC=1)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(is_msvc TRUE)
    list(APPEND BSTK_INTERNAL_DEFS _BSTK_MSVC=1)
else()
    # unsupported compiler, report an error and terminate
    message(FATAL_ERROR "Detected unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

# detect unsupported compiler and platform combination, terminate if not allowed
if(WIN32 AND NOT ${is_msvc})
    message(FATAL_ERROR "Only MSVC is supported on Windows.")
elseif(UNIX AND NOT ${is_gcc})
    message(FATAL_ERROR "Only GCC is supported on Linux.")
endif()

# detect the architecture and set the corresponding definition, terminate if the architecture is not supported
if(${BSTK_PLATFORM_ARCH} STREQUAL "x64")
    list(APPEND BSTK_INTERNAL_DEFS _BSTK_X64=1)
elseif(${BSTK_PLATFORM_ARCH} STREQUAL "x86")
    list(APPEND BSTK_INTERNAL_DEFS _BSTK_X86=1)
else()
    # unsupported architecture, report an error and terminate
    message(FATAL_ERROR "Unsupported platform architecture detected.")
endif()

# detect the byte order and set the corresponding definition, terminate if unable to detect the byte order
if(CMAKE_CXX_BYTE_ORDER STREQUAL "LITTLE_ENDIAN")
    list(APPEND BSTK_INTERNAL_DEFS _BSTK_LITTLE_ENDIAN=1)
elseif(CMAKE_CXX_BYTE_ORDER STREQUAL "BIG_ENDIAN")
    list(APPEND BSTK_INTERNAL_DEFS _BSTK_BIG_ENDIAN=1)
else()
    # failed to detect the byte order, report an error and terminate
    message(FATAL_ERROR "Byte order could not be detected.")
endif()

# detect support for 128-bit integers and set the corresponding definition (definitely not supported on MSVC)
if(${is_gcc})
    # use check_cxx_source_compiles() to detect whether a program that uses 128-bit integers will compile
    include(CheckCXXSourceCompiles)
    check_cxx_source_compiles([=[
        using _Int128_t  = __int128;
        using _Uint128_t = unsigned __int128;
        static_assert(sizeof(_Int128_t) == 16, "128-bit integer size mismatch");
        static_assert(sizeof(_Uint128_t) == 16, "128-bit integer size mismatch");

        int main() {
            return 0;
        }
    ]=] int128_supported)
    if(${int128_supported})
        # 128-bit integers are supported, append the corresponding macro
        list(APPEND BSTK_INTERNAL_DEFS _BSTK_INT128_SUPPORTED=1)
    endif()
endif()

# organize source files into directories for better readability
source_group("src/core" FILES ${BSTK_CORE_INC_FILES} ${BSTK_CORE_SRC_FILES})
source_group("src/core/impl" FILES ${BSTK_CORE_IMPL_FILES})
source_group("res" FILES ${BSTK_RES_FILES})

# add the BSTK shared library and adjust its properties
add_library(bstk SHARED ${BSTK_CORE_INC_FILES} ${BSTK_CORE_SRC_FILES} ${BSTK_CORE_IMPL_FILES})
target_compile_definitions(bstk PRIVATE _BSTK_BUILD=1 PUBLIC ${BSTK_INTERNAL_DEFS})
target_compile_features(bstk PRIVATE cxx_std_20)
target_include_directories(bstk PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(bstk PROPERTIES PREFIX "") # prevent compilers from adding "lib" prefix

# Note: RC files are used only on Windows and are compiled with rc.exe tool.
#       Therefore, they are included only when compiling for Windows.
if(WIN32)
    target_sources(bstk PRIVATE ${BSTK_RES_FILES})
endif()

# setup debug symbol generation for all supported compilers
if(${is_gcc})
    # generate DWARF symbols and extract them into a separate DEBUG file
    target_compile_options(bstk PRIVATE "-g")

    set(debug_file "$<TARGET_FILE_DIR:bstk>/$<TARGET_FILE_BASE_NAME:bstk>.debug")
    add_custom_command(TARGET bstk POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} --only-keep-debug $<TARGET_FILE:bstk> ${debug_file}
        COMMAND ${CMAKE_OBJCOPY} --add-gnu-debuglink=${debug_file} $<TARGET_FILE:bstk>
        COMMAND ${CMAKE_STRIP} --strip-debug $<TARGET_FILE:bstk>
    )
    unset(debug_file)
elseif(${is_msvc})
    # generate PDB file in both Debug and Release mode
    target_compile_options(bstk PRIVATE "/Zi")
    target_link_options(bstk PRIVATE "/DEBUG")
endif()