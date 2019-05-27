//
// Created by taras on 23.05.19.
//

#include "huffman_tree.h"

void huffman_tree::tree_delete(huffman_tree::node *v) {
    if (v->left != nullptr) {
        tree_delete(v->left);
    }
    if (v->right != nullptr) {
        tree_delete(v->right);
    }
    delete v;
}

void huffman_tree::build(std::array<symbol, ALPHABET_SIZE> freq) {
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

void huffman_tree::code_calculation(huffman_tree::node *v, bit_set &cur_code) {
    if (v == nullptr) {
        return;
    }
    if (v->is_leaf) {
        tree_code.push(1);
        symbol_map[v->ch] = cur_code;
        alphabet_code.append(cur_code);
    } else {
        tree_code.push(0);
        cur_code.push(0);
        code_calculation(v->left, cur_code);
        cur_code.pop();
        cur_code.push(1);
        code_calculation(v->right, cur_code);
        cur_code.pop();
    }
}

huffman_tree::huffman_tree(std::array<symbol, ALPHABET_SIZE> freq) {
    build(freq);
    bit_set buf;
    code_calculation(root, buf);
}

huffman_tree::~huffman_tree() {
    tree_delete(root);
}


