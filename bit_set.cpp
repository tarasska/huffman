//
// Created by taras on 25.05.19.
//

#include "bit_set.h"
bit_set::bit_set() : last_bit(0) {}

bit_set::bit_set(uint8_t byte) {
    data_.push_back(byte);
    last_bit = 0;
}

void bit_set::append(bit_set const& src) {
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
    //data_[old_size] <<= BYTE - last_bit;
    for (size_t i = old_size; i < data_.size() - 1; ++i) {
        data_[i] = (data_[i] << (BYTE - last_bit)) | (data_[i + 1] >> last_bit);
    }
    if (src.last_bit <= BYTE - last_bit && src.last_bit) {
        data_.pop_back();
        last_bit = (src.last_bit + last_bit) % BYTE;
    } else {
        data_.back() <<= (BYTE - last_bit);
        last_bit = (src.last_bit + last_bit) % BYTE;
    }
}

void bit_set::push(ElemType x) {
    if (last_bit == 0) {
        data_.push_back(0);
    }
    if (x == 0) {
        data_.back() &= FULL_BYTE - (1 << (BYTE - last_bit - 1));
    } else {
        data_.back() |= (x << (BYTE - last_bit - 1));
    }
    last_bit = (last_bit + 1) % BYTE;
}

void bit_set::pop() {
    if (last_bit == 1) {
        data_.pop_back();
        last_bit = 0;
    } else {
        last_bit = last_bit ? last_bit - 1 : MAX_BIT;
    }
}
std::vector<ElemType> const& bit_set::get_data() const {
    return data_;
}

size_t bit_set::get_bit_size() const {
    return data_.size() * BYTE - (last_bit == 0 ? 0 : BYTE - last_bit);
}

uint8_t bit_set::get_last_bit() const {
    return last_bit;
}

uint8_t bit_set::at(size_t index) const {
    uint8_t x = (data_[index / BYTE] >> (BYTE - index % BYTE - 1)) & 1u;
    return x;
}

void bit_set::cut_tail(size_t size) {
    for (size_t i = 0; i < size; ++i) {
        pop();
    }
}

std::string bit_set::to_string() const {
    std::string buffer;
    for (size_t i = 0; i < get_bit_size(); ++i) {
        buffer += static_cast<char>('0' + at(i));
    }
    return buffer;
}



