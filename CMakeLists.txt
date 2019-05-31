cmake_minimum_required(VERSION 3.12)
project(huffman)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread")
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")

add_library(huffman_lib STATIC type_def.h bit_set.h bit_set.cpp freq_counter.h freq_counter.cpp
        huffman_tree.h huffman_tree.cpp)

add_executable(huffman gtest/gtest.h gtest/gtest-all.cc gtest/gtest_main.cc test/testing_huffman.cpp)

add_executable(util utility.cpp)

target_link_libraries(huffman pthread huffman_lib)
target_link_libraries(util pthread huffman_lib)