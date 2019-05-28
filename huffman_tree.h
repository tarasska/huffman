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
        node* left, *right;
        ElemType ch;
        size_t cnt;
        bool is_leaf;
//        friend bool operator<(node const& a, node const& b) {
//            return a.cnt < b.cnt;
//        }
    };
    node* root;
    bit_set alphabet_code;
    bit_set tree_code;
    std::array<bit_set, ALPHABET_SIZE> symbol_map;

    static void tree_delete(node *v);
    void build(std::array<symbol, ALPHABET_SIZE> freq);
    void code_calculation(node* v, bit_set& cur_code);
  public:
    explicit huffman_tree(std::array<symbol, ALPHABET_SIZE> freq);
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

};

#endif //HUFFMAN__HUFFMAN_TREE_H_
