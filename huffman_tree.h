//
// Created by taras on 23.05.19.
//

#ifndef HUFFMAN__HUFFMAN_TREE_H_
#define HUFFMAN__HUFFMAN_TREE_H_

#include <array>
#include <queue>
#include "type_def.h"

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
  public:
    huffman_tree(std::array<symbol, ALPHABET_SIZE> freq);
    ~huffman_tree();

};

#endif //HUFFMAN__HUFFMAN_TREE_H_
