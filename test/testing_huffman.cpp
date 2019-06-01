//
// Created by taras on 31.05.19.
//

#include <random>
#include <assert.h>
#include <fstream>
#include <chrono>
#include "../gtest/gtest.h"
#include "../huffman_tree.h"

TEST(correctness, bit_set_constructor_uint8) {
    bit_set set(128);
    EXPECT_EQ(set.to_string(), "10000000");
    bit_set set2(0);
    EXPECT_EQ(set2.to_string(), "00000000");
    bit_set set3(255);
    EXPECT_EQ(set3.to_string(), "11111111");
    bit_set set4(228);
    EXPECT_EQ(set4.to_string(), "11100100");
}

TEST(correctness, bit_set_small) {
    bit_set set;
    for (size_t i = 0; i < 18; i += 2) {
        set.push(0);
        set.push(1);
    }
    EXPECT_EQ(set.to_string(), "010101010101010101");
    for (size_t i = 0; i < 17; ++i) {
        set.pop();
    }
    EXPECT_EQ(set.to_string(), "0");
    set.pop();
    EXPECT_EQ(set.to_string(), "");
    for (size_t i = 0; i < 8; ++i) {
        set.push(1);
    }
    EXPECT_EQ(set.get_bit_size(), 8);
    set.push(0);
    set.push(0);
    EXPECT_EQ(set.get_bit_size(), 10);
    EXPECT_EQ(set.to_string(), "1111111100");
}

TEST(correctness, bit_set_append) {
    bit_set set1, set2;
    for (size_t i = 0; i < 18; i += 2) {
        set1.push(0);
        set2.push(1);
    }
    set1.append(set2);
    EXPECT_EQ(set1.to_string(), "000000000111111111");
    bit_set set3(228);
    EXPECT_EQ(set3.to_string(), "11100100");
    set2.append(set3);
    EXPECT_EQ(set2.to_string(), "11111111111100100");
    bit_set set4(99);
    set4.append(bit_set(38));
    EXPECT_EQ(set4.to_string(), "0110001100100110");
}

void vector_test(size_t test_size) {
    std::vector<uint8_t> data;
    for (size_t i = 0; i < test_size; ++i) {
        data.push_back(static_cast<uint8_t>(i % 256));
    }
    freq_counter fc;
    fc.update(data.begin(), data.end());
    huffman_tree tree(fc);
    auto encode = tree.get_header_code();
    size_t m = 0;
    size_t size_enc = 0;
    while (true) {
        if ((m + 1) * BLOCK_SIZE >= size_enc) {
            encode.append(tree.encode_block(data.begin() + m * BLOCK_SIZE, data.end()));
            break;
        } else {
            encode.append(tree.encode_block(data.begin() + m * BLOCK_SIZE,
                data.begin() + (m + 1) * BLOCK_SIZE));
            ++m;
        }
    }
    m = 0;
    std::string result;
    size_t size_dec = encode.size();
    while (true) {
        if ((m + 1) * BLOCK_SIZE >= size_dec) {
            result.append(tree.decode(encode.begin() + m * BLOCK_SIZE, encode.end()));
            break;
        } else {
            result.append(tree.decode(encode.begin() + m * BLOCK_SIZE,
                                            encode.begin() + (m + 1) * BLOCK_SIZE));
            ++m;
        }
    }
    EXPECT_EQ(data.size(), result.size());
    for (size_t i = 0; i < data.size(); ++i) {
        EXPECT_EQ(data[i], FULL_BYTE & result[i]);
    }
}

TEST(correctness, vector_small) {
    vector_test(10000);
}

TEST(correctness, vector_big) {
    vector_test(1000000);
}

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
        data.push_back((char) ('a' + distribution(gen) % 26));
    }
    freq_counter fc;
    fc.update(data.begin(), data.end());
    huffman_tree tree(fc);
    std::string s = tree.get_header_code();
    s.append(tree.encode_block(data.begin(), data.end()));
    std::string result = tree.decode(s.begin(), s.end());
    data.push_back('\0');
    std::string expect(data.data());
    EXPECT_EQ(expect, result);
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
    out.open("test/buffer_file_for_encode.out", std::ios::out | std::ios::binary);
    if (!in.is_open() || !out.is_open()) {
        std::cerr << "in/out stream failed" << std::endl;
        return;
    }
    freq_counter fc;
    std::vector<char> input;
    in.read(input.data(), BLOCK_SIZE);
    fc.update(input.begin(), input.end());
    huffman_tree tree(fc);
    std::string encode = tree.get_header_code() + tree.encode_block(input.begin(), input.end());
    out.write(encode.data(), encode.size());
    in.close();
    out.close();
    in.open("test/buffer_file_for_encode.out", std::ios::in | std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "result stream failed" << std::endl;
        return;
    }
    huffman_tree decode_tree;
    std::vector<char> encode_data;
    in.read(encode_data.data(), BLOCK_SIZE);
    std::string result = decode_tree.decode(encode_data.begin(), encode_data.end());
    EXPECT_EQ("", result);
    in.close();
}

void generate_input_file_from_vocabulary(size_t word_cnt_in_file) {
    std::vector<std::string> vocabulary;
    std::vector<std::string> whitespace = {" ", "\n"};
    std::ifstream in;
    in.open("test/vocabulary.in", std::ios::in | std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "vocabulary stream failed";
    }
    std::string word;
    while (!in.eof()) {
        in >> word;
        vocabulary.push_back(word);
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        vocabulary.push_back(word);
    }
    std::ofstream out;
    out.open("test/input_file_for_random.in", std::ios::out | std::ios::binary);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> distribution(0, vocabulary.size() - 1);
    for (size_t i = 0; i < word_cnt_in_file; ++i) {
        size_t id = distribution(gen);
        out.write(vocabulary[id].data(), vocabulary[id].size());
        out.write(whitespace[0].data(), 1);
        if (i % 25 == 0) {
            out.write(whitespace[1].data(), 1);
        }
    }

}

void compress(std::string input_file, std::string output_file) {
    auto time_in = std::chrono::high_resolution_clock::now();
    std::ifstream in;
    in.open(input_file, std::ios::in | std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "unable to open input file";
        return;
    }
    std::ofstream out;
    out.open(output_file, std::ios::out | std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "unable to open output file";
        return;
    }
    size_t input_size = 0;
    size_t output_size = 0;
    freq_counter fc;
    std::vector<char> data(BLOCK_SIZE);
    while (!in.eof()) {
        in.read(data.data(), data.size());
        fc.update(data.begin(), data.begin() + in.gcount());
        input_size += in.gcount();
    }

    in.clear();
    in.seekg(0);

    huffman_tree tree(fc);
    auto header = tree.get_header_code();
    out.write(header.data(), header.size());
    output_size += header.size();
    while (!in.eof()) {
        in.read(data.data(), data.size());
        auto block = tree.encode_block(data.begin(), data.begin() + in.gcount());
        out.write(block.data(), block.size());
        output_size += block.size();
    }
    std::chrono::duration<double> dur = std::chrono::high_resolution_clock::now() - time_in;
    std::cout << "encoding completed successfully in " << dur.count()
              << "\n"
              << input_file << ":" << input_size << " bytes, " << input_size / (1 << 20) << " Mb\n"
              << output_file << ":" << output_size << " bytes, " << output_size / (1 << 20) << " Mb"
              << std::endl;
}

void decompress(std::string input_file, std::string output_file) {
    auto time_in = std::chrono::high_resolution_clock::now();
    std::ifstream in;
    in.open(input_file, std::ios::in | std::ios::binary);
    if (!in.is_open()) {
        std::cerr << "unable to open input file";
        return;
    }
    std::ofstream out;
    out.open(output_file, std::ios::out | std::ios::binary);
    if (!out.is_open()) {
        std::cerr << "unable to open output file";
        return;
    }
    size_t input_size = 0;
    size_t output_size = 0;
    std::vector<char> data(BLOCK_SIZE);
    huffman_tree tree;

    while (!in.eof()) {
        in.read(data.data(), data.size());
        input_size += in.gcount();
        auto block = tree.decode(data.begin(), data.begin() + in.gcount());
        out.write(block.data(), block.size());
        output_size += block.size();
    }
    std::chrono::duration<double> dur = std::chrono::high_resolution_clock::now() - time_in;
    std::cout << "decoding completed successfully in " << dur.count()
              << "\n"
              << input_file << ":" << input_size << " bytes, " << input_size / (1 << 20) << " Mb\n"
              << output_file << ":" << output_size << " bytes, " << output_size / (1 << 20) << " Mb"
              << std::endl;
}

bool is_file_equals(std::string expect_file, std::string decode_file) {
    std::ifstream in_expect, in_decode;
    in_expect.open(expect_file, std::ios::in | std::ios::binary);
    if (!in_expect.is_open()) {
        std::cerr << "expect stream failed";
    }
    in_decode.open(decode_file, std::ios::in | std::ios::binary);
    if (!in_expect.is_open()) {
        std::cerr << "decode stream failed";
    }
    std::vector<char> exp_data(BLOCK_SIZE), dec_data(BLOCK_SIZE);
    size_t exp_it = 0, dec_it = 0;
    size_t exp_size = 0, dec_size = 0;
    while (!in_expect.eof() && !in_decode.eof()) {
        if (exp_it >= exp_size) {
            in_expect.read(exp_data.data(), BLOCK_SIZE);
            exp_size = in_expect.gcount();
        }
        if (dec_it >= dec_size) {
            in_decode.read(dec_data.data(), BLOCK_SIZE);
            dec_size = in_decode.gcount();
        }
        while (exp_it < exp_size && dec_it < dec_size) {
            if (exp_data[exp_it] != dec_data[dec_it]) {
                in_expect.close();
                in_decode.close();
                return false;
            }
            ++exp_it;
            ++dec_it;
        }
    }
    bool ret = in_expect.eof() && in_decode.eof();
    in_expect.close();
    in_decode.close();
    return ret;
}

TEST(correctness, random_file_5mb) {
    generate_input_file_from_vocabulary(1000000);
    compress("test/input_file_for_random.in", "test/buffer_file_for_encode.out");
    decompress("test/buffer_file_for_encode.out", "test/file_for_decode.out");
    ASSERT_TRUE(is_file_equals("test/input_file_for_random.in", "test/file_for_decode.out"));
}

TEST(correctness, random_file_59mb) {
    generate_input_file_from_vocabulary(10000000);
    compress("test/input_file_for_random.in", "test/buffer_file_for_encode.out");
    decompress("test/buffer_file_for_encode.out", "test/file_for_decode.out");
    ASSERT_TRUE(is_file_equals("test/input_file_for_random.in", "test/file_for_decode.out"));
}

//TEST(correctness, random_file_592mb) {
//    generate_input_file_from_vocabulary(100000000);
//    compress("test/input_file_for_random.in", "test/buffer_file_for_encode.out");
//    decompress("test/buffer_file_for_encode.out", "test/file_for_decode.out");
//    ASSERT_TRUE(is_file_equals("test/input_file_for_random.in", "test/file_for_decode.out"));
//}


TEST(correctness, Dostoevsky_file) {
    compress("test/grand_inquisitor.txt", "test/buffer_file_for_encode.out");
    decompress("test/buffer_file_for_encode.out", "test/file_for_decode.out");
    ASSERT_TRUE(is_file_equals("test/grand_inquisitor.txt", "test/file_for_decode.out"));
}

TEST(correctness, Gogol_file) {
    compress("test/taras_bulba.txt", "test/buffer_file_for_encode.out");
    decompress("test/buffer_file_for_encode.out", "test/file_for_decode.out");
    ASSERT_TRUE(is_file_equals("test/taras_bulba.txt", "test/file_for_decode.out"));
}