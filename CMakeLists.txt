cmake_minimum_required(VERSION 3.12)

project(textable)
set (CMAKE_CXX_STANDARD 11)

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

enable_testing()
add_test(NAME Test COMMAND textable_test)

