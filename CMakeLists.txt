cmake_minimum_required(VERSION 3.9)

project(textable)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# By default, Visual Studio detects a byte-order mark to determine if the source
# file is in an encoded Unicode format, for example, UTF-16 or UTF-8. If no
# byte-order mark is found, it assumes the source file is encoded using the
# current user code page, unless you've specified a code page by using /utf-8
if (MSVC)
    # Instruct Visual Studio to use proper char set for both source and binary files.
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

add_library(textable
  src/textable.h
  src/textable.cpp
)

add_executable(textable_test
  test/main.cpp
)

# Includes
target_include_directories(textable_test PRIVATE src)

enable_testing()

find_package(GTest CONFIG REQUIRED)
target_link_libraries(textable_test PRIVATE textable GTest::gtest)

add_test(NAME Test COMMAND textable_test)

# Copy GTest libraries to the target directory.
add_custom_command(
    TARGET textable_test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:GTest::gtest>
        $<TARGET_FILE_DIR:textable_test>
)

# Coverage support.
option(CODE_COVERAGE "Enable coverage reporting" ON)
if (CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  # Add required flags (GCC & LLVM/Clang)
  target_compile_options(textable INTERFACE
    -O0        # no optimization
    -g         # generate debug info
    --coverage # sets all required flags
  )
  if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(textable INTERFACE --coverage)
  else()
    target_link_libraries(textable INTERFACE --coverage)
  endif()
endif()

