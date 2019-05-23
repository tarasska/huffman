//
// Created by taras on 22.05.19.
//
#include "file_reader.h"
#include <exception>

void file_reader::buffer_read() {
    in_.read(data_, BUFFER_SIZE_);
    current_size_ = in_.gcount();
}

file_reader::file_reader(char const *file) {
    in_.open(file, std::ios::in | std::ios::binary);
    if (!in_.is_open()) {
        throw std::runtime_error("could not open file");
    }
    data_ = new ElemType[BUFFER_SIZE_];
    current_it_ = 0;
    current_size_ = 0;
}

file_reader::~file_reader() {
    delete[] data_;
    in_.close();
}

void file_reader::read() {
    if (current_it_ >= current_size_) {
        buffer_read();
    }
}

ElemType file_reader::get_elem() {
    if (current_it_ < current_size_) {
        return data_[current_it_++];
    } else {
        buffer_read();
    }
}
bool file_reader::eof() const {
    return in_.eof();
}
ElemType const *file_reader::get_buffer() const {
    return data_ + current_it_;
}
size_t file_reader::get_size() const {
    return current_size_;
}









