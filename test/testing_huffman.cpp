//
// Created by taras on 31.05.19.
//

#include <random>
#include <fstream>
#include "../gtest/gtest.h"
#include "../huffman_tree.h"

TEST(correctness, const_string) {
    std::string input = "Тарас хороший программист";
    std::string bad_input = "тарас хороший программист";
    freq_counter fc;
    fc.update(input.begin(), input.end());
    huffman_tree tree(fc);
    std::string code = tree.get_header_code() + tree.encode_block(input.begin(), input.end());
    EXPECT_EQ(tree.decode(code.begin(), code.end()), input);
    EXPECT_NE(bad_input, code);
}

TEST(correctness, random_small_string) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 1000);
    std::vector<char> data;
    for (size_t i = 0; i < 1000; ++i) {
        data.push_back((char) ('a' + distribution(gen) % 20));
    }
    freq_counter fc;
    fc.update(data.begin(), data.end());
    huffman_tree tree(fc);
    std::string s = tree.get_header_code();
    s.append(tree.encode_block(data.begin(), data.end()));
    std::string result = tree.decode(s.begin(), s.end());
    std::string expect = data.data();
    EXPECT_EQ(result, expect);
}

TEST(correctness, big_block_in_one_string_random_string_test) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, 10000);
    const size_t block_cnt = 5;
    std::string input_data;
    freq_counter fc;
    while (input_data.size() < block_cnt * BLOCK_SIZE) {
        std::string buffer;
        for (size_t i = 0; i < BLOCK_SIZE; ++i) {
            buffer += static_cast<char>(distribution(gen) % ALPHABET_SIZE);
        }
        fc.update(buffer.begin(), buffer.end());
        input_data.append(buffer);
    }
    huffman_tree tree(fc);
    std::string encode = tree.get_header_code();
    size_t m = 0;
    while (m < block_cnt) {
        encode.append(tree.encode_block(input_data.begin() + m * BLOCK_SIZE,
                                        input_data.begin() + (m + 1) * BLOCK_SIZE));
        ++m;
    }
    huffman_tree decode_tree;
    std::string result;
    m = 0;
    while (encode.begin() + m * BLOCK_SIZE < encode.end()) {
        if (encode.begin() + (m + 1) * BLOCK_SIZE > encode.end()) {
            result.append(decode_tree.decode(encode.begin() + m * BLOCK_SIZE, encode.end()));
            break;
        } else {
            result.append(decode_tree.decode(encode.begin() + m * BLOCK_SIZE,
                                             encode.begin() + (m + 1) * BLOCK_SIZE));
        }
        ++m;
    }
    EXPECT_EQ(input_data.size(), result.size());
    for (size_t i = 0; i < input_data.size(); ++i) {
        if (input_data[i] != result[i]) {
            std::cout << i << " ";
        }
    }

    EXPECT_EQ(input_data, result);
}

TEST(correctness, empty_file) {
    std::ifstream in;
    std::ofstream out;
    in.open("test/empty_file.in", std::ios::in | std::ios::binary);
    out.open("buffer_file_for_encode.out", std::ios::out | std::ios::binary);
    if (!in.is_open() || !out.is_open()) {
        std::cerr << "stream failed" << std::endl;
        return;
    }
    freq_counter fc;
    std::string input;

}

