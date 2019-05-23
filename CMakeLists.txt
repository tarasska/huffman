cmake_minimum_required(VERSION 3.12)
project(huffman)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -lpthread")
add_executable(huffman gtest/gtest.h gtest/gtest-all.cc gtest/gtest_main.cc file_reader.cpp file_reader.h const_def.h huffman_tree.cpp huffman_tree.h)