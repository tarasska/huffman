//
// Created by taras on 23.05.19.
//

#ifndef HUFFMAN__HUFFMAN_TREE_H_
#define HUFFMAN__HUFFMAN_TREE_H_

#include <array>
#include "const_def.h"

class huffman_tree {
  private:

  public:
    huffman_tree(std::array<uint32_t, ALPHABET_SIZE> freq);
    ~huffman_tree();


};

#endif //HUFFMAN__HUFFMAN_TREE_H_
