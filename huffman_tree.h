//
// Created by taras on 23.05.19.
//

#ifndef HUFFMAN__HUFFMAN_TREE_H_
#define HUFFMAN__HUFFMAN_TREE_H_

#include <algorithm>
#include <array>
#include <queue>
#include "type_def.h"
#include "bit_set.h"

#define BLOCK_SIZE 65536

class huffman_tree {
  private:
    struct node {
        node* left, *right, *parent;
        char ch;
        size_t cnt;
        bool is_leaf;
    };

    struct decoder_box {
        std::string state = "header_alpha";
        size_t current_size;
        node* v_in_tree = nullptr;
    } decoder_box_;

    node* root;
    bit_set alphabet_code;
    bit_set tree_code;
    std::array<bit_set, ALPHABET_SIZE> symbol_map;

    node* decoder_root;
    std::queue<char> decoder_alphabet;
    bit_set dfs_tree_code;
    size_t dfs_tree_size;
    std::string buffer_block;

    static void tree_delete(node *v);
    void build(std::array<symbol, ALPHABET_SIZE> freq);
    void code_calculation(node* v, bit_set& cur_code);
    void build_decoder_tree();
    void tree_decode_block(bit_set& block_code);

    template <typename InputIt>
    InputIt decode_alpha(InputIt first, InputIt last) {
        while (first != last && decoder_alphabet.size() < decoder_box_.current_size) {
            decoder_alphabet.push(*first);
            first++;
        }
    }

    template <typename InputIt>
    InputIt decode_tree(InputIt first, InputIt last) {
        while (first != last && dfs_tree_code.get_bit_size() < decoder_box_.current_size) {
            dfs_tree_code.append(bit_set(*first));
            first++;
        }
    }

    template <typename InputIt>
    std::string const& decode_block(InputIt first, InputIt last) {
        bit_set block_code;
        while (first != last) {
            if (decoder_box_.current_size == 0) {
                decoder_box_.current_size = *first;
                first++;
            }
            while (first != last && dfs_tree_size < decoder_box_.current_size) {
                block_code.append(bit_set(*first));
                first++;
            }
            if (dfs_tree_size >= decoder_box_.current_size) {
                decoder_box_.current_size = 0;
            }
        }
        tree_decode_block(block_code);
        return buffer_block;
    }


  public:
    explicit huffman_tree(std::array<symbol, ALPHABET_SIZE> freq);
    huffman_tree() = default;
    ~huffman_tree();

    std::string get_header_code() const;

    template <typename InputIt>
    std::string encode_block(InputIt first, InputIt last) {
        if (!std::is_trivially_copyable<typename std::iterator_traits<InputIt>::value_type>::value) {
            throw std::runtime_error("not trivially copyable type is prohibited");
        }
        static_assert(sizeof(typename std::iterator_traits<InputIt>::value_type), "not 1 byte");
        bit_set block;
        while (first != last) {
            block.append(symbol_map[*first]);
            first++;
        }
        std::string result;
        result.push_back(block.get_bit_size());
        std::move(block.get_data().begin(), block.get_data().end(), std::back_inserter(result));
        return result;
    }

    template <typename InputIt>
    std::string decode(InputIt first, InputIt last) {
        if (!std::is_trivially_copyable<typename std::iterator_traits<InputIt>::value_type>::value) {
            throw std::runtime_error("not trivially copyable type is prohibited");
        }
        static_assert(sizeof(typename std::iterator_traits<InputIt>::value_type), "not 1 byte");
        if (decoder_box_.state == "header_alpha") {
            if (decoder_box_.current_size == 0) {
                decoder_box_.current_size = *first;
                first++;
            }
            first = decode_alpha(first, last);
            if (decoder_alphabet.size() == decoder_box_.current_size) {
                decoder_box_.state = "header_tree";
                decoder_box_.current_size = 0;
            } else {
                return "";
            }
        }
        if (decoder_box_.state == "header_tree") {
            if (decoder_box_.current_size == 0) {
                decoder_box_.current_size = *first;
                first++;
            }
            first = decode_tree(first, last);
            if (dfs_tree_code.get_bit_size() >= decoder_box_.current_size) {
                dfs_tree_code.cut_tail(decoder_box_.current_size % BYTE);
                build_decoder_tree();
                decoder_box_.state = "block";
                decoder_box_.current_size = 0;
            } else {
                return "";
            }
        }
        if (decoder_box_.state == "block") {
            return decode_block(first, last);
        }
    }

};

#endif //HUFFMAN__HUFFMAN_TREE_H_
