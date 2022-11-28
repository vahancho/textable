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

set(TARGET textable_test)

find_package(GTest CONFIG REQUIRED)

add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} textable)
target_link_libraries(${TARGET} GTest::gtest)

add_test(NAME Test COMMAND ${TARGET})

# Copy GTest libraries to the target directory.
add_custom_command(
    TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:GTest::gtest>
            $<TARGET_FILE_DIR:${TARGET}>
)

# Coverage support.
option(CODE_COVERAGE "Enable coverage reporting" ON)
if (CODE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    # Add required flags (GCC & LLVM/Clang)
    target_compile_options(polytest PUBLIC
        -O0        # no optimization
        -g         # generate debug info
        --coverage # sets all required flags
    )
    if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
        target_link_options(${TARGET} PUBLIC --coverage)
    else()
        target_link_libraries(${TARGET} PUBLIC --coverage)
    endif()
endif()
