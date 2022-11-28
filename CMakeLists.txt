#**********************************************************************************
#  MIT License                                                                    *
#                                                                                 *
#  Copyright (c) 2022 Vahan Aghajanyan <vahancho@gmail.com>                       *
#                                                                                 *
#  Permission is hereby granted, free of charge, to any person obtaining a copy   *
#  of this software and associated documentation files (the "Software"), to deal  *
#  in the Software without restriction, including without limitation the rights   *
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell      *
#  copies of the Software, and to permit persons to whom the Software is          *
#  furnished to do so, subject to the following conditions:                       *
#                                                                                 *
#  The above copyright notice and this permission notice shall be included in all *
#  copies or substantial portions of the Software.                                *
#                                                                                 *
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR     *
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,       *
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE    *
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER         *
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,  *
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE  *
#  SOFTWARE.                                                                      *
#*********************************************************************************/

cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

project(textable VERSION 1.1.0
        DESCRIPTION "A C++ implementation of a plain text tables generator."
        LANGUAGES CXX)

# Make cache variables for install destinations
include(GNUInstallDirs)

# General options
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(ENABLE_TESTING "Enable unit test build" OFF)

# By default, Visual Studio detects a byte-order mark to determine if the source
# file is in an encoded Unicode format, for example, UTF-16 or UTF-8. If no
# byte-order mark is found, it assumes the source file is encoded using the
# current user code page, unless you've specified a code page by using /utf-8
if (MSVC)
    # Instruct Visual Studio to use proper char set for both source and binary files.
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

if (NOT MSVC)
    # Specify the target architecture (Linux). For Windows based generator use rather
    # '-A Win32' or '-A x64 options'
    set(TARGET_ARCH x86 CACHE STRING "the target architecture")
    set_property(CACHE TARGET_ARCH PROPERTY STRINGS x86 x64)

    if (TARGET_ARCH STREQUAL "x86")
        set(CMAKE_C_FLAGS   -m32)
        set(CMAKE_CXX_FLAGS -m32)
    elseif(TARGET_ARCH STREQUAL "x64")
        set(CMAKE_C_FLAGS   -m64)
        set(CMAKE_CXX_FLAGS -m64)
    else()
        message(FATAL_ERROR "Incorrect target architecture specified. "
                            "It should be either x86 or x64")
    endif()

    # Get the architecture: x86 vs x64
    if(TARGET_ARCH STREQUAL "x64")
        set(BIT "x86_64")
    else()
        set(BIT "x86")
    endif()

    add_compile_options(-Wall -Wextra -pedantic -Werror)
else()
    # By default, Visual Studio detects a byte-order mark to determine if the source
    # file is in an encoded Unicode format, for example, UTF-16 or UTF-8. If no
    # byte-order mark is found, it assumes the source file is encoded using the
    # current user code page, unless you've specified a code page by using /utf-8

    # Instruct Visual Studio to use proper char set for both source and binary files.
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

# Append a postfix for the debug libraries
if (NOT CMAKE_DEBUG_POSTFIX)
    set(CMAKE_DEBUG_POSTFIX d)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)

if (ENABLE_TESTING)
    enable_testing()
    include(Dart)

    add_subdirectory(test)
endif()
