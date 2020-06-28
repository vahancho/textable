cmake_minimum_required(VERSION 3.12)

project(textable)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(textable
  src/textable.h
  src/textable.cpp
)

add_executable(textable_test
  test/main.cpp
)

target_link_libraries(textable_test PRIVATE textable)

# Includes
target_include_directories(textable_test PRIVATE src)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # The test application also need to be compiled with the same flags.
    target_compile_options(textable_test INTERFACE LINK_FLAGS --coverage)
endif()

enable_testing()
add_test(NAME Test COMMAND textable_test)

