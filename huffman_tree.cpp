//
// Created by taras on 23.05.19.
//

#include "huffman_tree.h"
huffman_tree::huffman_tree(std::array<symbol, ALPHABET_SIZE> freq) {
    auto my_comp = [](node const* a, node const* b) {return (*a).cnt < (*b).cnt;};
    std::priority_queue<node*,  std::vector<node*>, decltype(my_comp)> q(my_comp);
    for (symbol const& elem : freq) {
        q.push(new node {nullptr, nullptr, elem.ch, elem.cnt, true});
    }
    while (q.size() > 1) {
        node* ptr_l = q.top();
        q.pop();
        node* ptr_r = q.top();
        q.pop();
        q.push(new node {ptr_l, ptr_r, 0, (*ptr_l).cnt + (*ptr_r).cnt, false});
    }
    root = q.top();
    q.pop();
}
