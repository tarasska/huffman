//
// Created by taras on 22.05.19.
//

#ifndef HUFFMAN__FILE_READER_H_
#define HUFFMAN__FILE_READER_H_
#include <cstdio>
#include <array>
#include <fstream>
#include "type_def.h"

class file_reader {
  private:
    static const size_t BUFFER_SIZE_ = 256;
    ElemType *data_;
    std::basic_ifstream<ElemType> in_;
    size_t current_size_;
    size_t current_it_;

    void buffer_read();

  public:
    file_reader(char const* file);
    ~file_reader();

    void read();
    ElemType get_elem();
    bool eof() const;
    ElemType const* get_buffer() const;
    size_t get_size() const;
};

#endif //HUFFMAN__FILE_READER_H_
