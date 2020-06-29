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
    # Add required flags (GCC & LLVM/Clang)
    target_compile_options(textable_test INTERFACE
        -O0        # no optimization
        -g         # generate debug info
        --coverage # sets all required flags
    )

    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
        target_link_options(textable_test INTERFACE --coverage)
    else()
        target_link_libraries(textable_test INTERFACE --coverage)
    endif()
endif()

enable_testing()
add_test(NAME Test COMMAND textable_test)

