//
// Created by taras on 23.05.19.
//

#include "huffman_tree.h"

void huffman_tree::tree_delete(huffman_tree::node* v) {
    if (v != nullptr) {
        tree_delete(v->left);
        tree_delete(v->right);
    }
    delete v;
}

void huffman_tree::build(std::array<symbol, ALPHABET_SIZE> freq) {
    auto my_comp = [](node const* a, node const* b) { return (*a).cnt < (*b).cnt; };
    std::priority_queue<node*, std::vector<node*>, decltype(my_comp)> q(my_comp);
    for (symbol const& elem : freq) {
        if (elem.cnt <= 0) {
            continue;
        }
        q.push(new node{nullptr, nullptr, nullptr, static_cast<char>(elem.ch), elem.cnt, true});
    }
    while (q.size() > 1) {
        node* ptr_l = q.top();
        q.pop();
        node* ptr_r = q.top();
        q.pop();
        q.push(new node{ptr_l, ptr_r, nullptr, 0, (*ptr_l).cnt + (*ptr_r).cnt, false});
    }
    root = q.top();
    q.pop();
}

void huffman_tree::code_calculation(huffman_tree::node* v, bit_set& cur_code) {
    if (v == nullptr) {
        return;
    }
    if (v->is_leaf) {
        symbol_map[v->ch] = cur_code;
        alphabet_code.append(bit_set(v->ch));
    } else {
        tree_code.push(0);
        cur_code.push(0);
        code_calculation(v->left, cur_code);
        cur_code.pop();
        tree_code.push(1);
        code_calculation(v->right, cur_code);
        cur_code.pop();
    }
}

void huffman_tree::build_decoder_tree() {
    dfs_tree_size = dfs_tree_code.get_bit_size();
    if (dfs_tree_size == 1) {
        decoder_root = new node{nullptr, nullptr, nullptr, decoder_alphabet.front(), 366, true};
        decoder_alphabet.pop();
    } else {
        decoder_root = new node{nullptr, nullptr, nullptr, 0, 366, false};
    }
    size_t ind = 1;
    node* v = decoder_root;
    while (ind < dfs_tree_size) {
        if (dfs_tree_code.at(ind) == 0) {
            v->left = new node{nullptr, nullptr, v, 0, 366, false};
            v = v->left;
        } else {
            v->is_leaf = true;
            v->ch = decoder_alphabet.front();
            decoder_alphabet.pop();
            v = v->parent;
            while (v->right != nullptr && v->parent != nullptr) {
                v = v->parent;
            }
            if (v->right != nullptr) {
                throw std::runtime_error("tree code is incorrect");
            } else {
                v->right = new node{nullptr, nullptr, v, 0, 366, false};
                v = v->right;
            }
        }
    }
    if (v->left != nullptr || v->right != nullptr) {
        throw std::runtime_error("tree code is incorrect");
    } else {
        v->is_leaf = true;
        v->ch = decoder_alphabet.front();
        decoder_alphabet.pop();
    }
}

huffman_tree::huffman_tree(std::array<symbol, ALPHABET_SIZE> freq)
    : root(nullptr), decoder_root(nullptr), dfs_tree_size(0) {
    build(freq);
    bit_set buf;
    buf.push(0);
    code_calculation(root, buf);
}

huffman_tree::~huffman_tree() {
    tree_delete(root);
    tree_delete(decoder_root);
}

std::string huffman_tree::get_header_code() const {
    auto tree_code_data_ = tree_code.get_data();
    auto alp_code_data_ = alphabet_code.get_data();
    std::string result;
    result.reserve(2 + alp_code_data_.size() + tree_code_data_.size());
    result.push_back(alp_code_data_.size());
    result.insert(result.end(), alp_code_data_.begin(), alp_code_data_.end());
    result.push_back(tree_code.get_bit_size());
    result.insert(result.end(), tree_code_data_.begin(), tree_code_data_.end());
    return result;
}



