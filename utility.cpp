//
// Created by taras on 30.05.19.
//

#include <iostream>
#include <cstring>
#include <fstream>
#include <chrono>
#include "huffman_tree.h"

int main(int argc, char* argv[]) {
    if (argc < 2 || (argc >= 2 && argc != 4 && strcmp(argv[1], "-h") != 0)) {
        std::cout << "Please, try again. For information try: ./program -h" << std::endl;
        return 0;
    }
    if (strcmp(argv[1], "-h") == 0) {
        std::cout << "for encode use: ./program -e input_file encode_file\n"
                  << "for decode use: ./program -d encode_file decode_file" << std::endl;
        return 0;
    }
    try {
        if (strcmp(argv[1], "-e") == 0) {
            auto time_in = std::chrono::high_resolution_clock::now();
            std::ifstream in;
            in.open(argv[2], std::ios::in | std::ios::binary);
            if (!in.is_open()) {
                std::cout << "unable to open input file";
                return 0;
            }
            std::ofstream out;
            out.open(argv[3], std::ios::out | std::ios::binary);
            if (!out.is_open()) {
                std::cout << "unable to open output file";
                return 0;
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
                      << argv[2] << ":" << input_size << " bytes\n"
                      << argv[3] << ":" << output_size << " bytes" << std::endl;
        } else if (strcmp(argv[1], "-d") == 0) {
            auto time_in = std::chrono::high_resolution_clock::now();
            std::ifstream in;
            in.open(argv[2], std::ios::in | std::ios::binary);
            if (!in.is_open()) {
                std::cout << "unable to open input file";
                return 0;
            }
            std::ofstream out;
            out.open(argv[3], std::ios::out | std::ios::binary);
            if (!out.is_open()) {
                std::cout << "unable to open output file";
                return 0;
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
                      << argv[2] << ":" << input_size << " bytes\n"
                      << argv[3] << ":" << output_size << " bytes" << std::endl;
        } else {
            std::cout << "Unknown key \"" << argv[1]
                      << "\"Please, try again. For information try: ./program -h" << std::endl;
        }
    } catch (std::runtime_error& re) {
        std::cout << "sorry, something went wrong with msg: " << re.what() << std::endl;
    }
}

