//
// Created by taras on 25.05.19.
//

#ifndef HUFFMAN__BIT_SET_H_
#define HUFFMAN__BIT_SET_H_
#include <vector>
#include <string>
#include <cstddef>
#include "type_def.h"

class bit_set {
  private:
    std::vector<ElemType> data_;
    uint8_t last_bit;
  public:
    bit_set();
    bit_set(bit_set const& other) = default;
    explicit bit_set(uint8_t bit);
    ~bit_set() = default;

    bit_set& operator=(bit_set const& src) = default;

    void append(bit_set const&);
    void push(ElemType);
    void pop();
    std::vector<ElemType> const& get_data() const;
    size_t get_bit_size() const;
    uint8_t get_last_bit() const;
    uint8_t at(size_t index) const;
    void cut_tail(size_t size);
    std::string to_string() const;

};

#endif //HUFFMAN__BIT_SET_H_
