# The MIT License (MIT)

# Copyright (c) 2023 Pierre Pharel Angoro Abiaga

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.

cmake_minimum_required (VERSION 3.10)

project (Tests LANGUAGES C CXX)

if (CMAKE_COMPILER_IS_GNUC)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-function")
elseif (CMAKE_COMPILER_IS_GNUCXX)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-function -Wold-style-cast -Wcast-qual")
elseif (MSVC)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Wall /W2 /wd4514")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /Wall /W2 /wd4514")
elseif (CMAKE_COMPILER_IS_CLANG)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")
else ()
    message (WARNING "Unknown compiler !")
endif ()

option(LOGGER "Log mode activated" OFF)

if(LOGGER)
  add_definitions(-DUTF8_DECODER_LOG)
endif()

set(LIB_DIR ..)

set(TESTS tests/)

include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_executable (testc ${LIB_DIR}/utf8_decoder.c ${TESTS}/tests.c)