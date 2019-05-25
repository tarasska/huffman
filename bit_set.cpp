//
// Created by taras on 25.05.19.
//

#include "bit_set.h"

void bit_set::append(bit_set const & src) {
    if (data_.empty()) {
        *this = src;
        return;
    }
    if (src.data_.empty()) {
        return;
    }
    size_t old_size = data_.size();
    data_.resize(old_size + src.data_.size());
    std::copy(src.data_.begin(), src.data_.end(), data_.begin() + old_size);
    if (last_bit == 0) {
        last_bit = src.last_bit;
        return;
    }
    data_[old_size - 1] |= data_[old_size] >> (last_bit);
    data_[old_size] <<= BYTE - last_bit;
    if (src.data_.size() == 1)
        return;
    for (size_t i = old_size; i < data_.size() - 1; ++i) {
        data_[i] = (data_[i] << (BYTE - last_bit)) | (data_[i + 1] >> last_bit);
    }
    if (src.last_bit <= BYTE - last_bit) {
        data_.pop_back();
        last_bit = 0;
    } else {
        data_.back() <<= (BYTE - last_bit);
        last_bit = src.last_bit - last_bit;
    }
}
void bit_set::push(ElemType x) {
    if (last_bit == 0) {
        data_.push_back(0);
    }
    data_.back() |= x << (BYTE - last_bit - 1);
    last_bit = (last_bit + 1) % BYTE;
}


