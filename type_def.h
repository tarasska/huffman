//
// Created by taras on 22.05.19.
//

#ifndef HUFFMAN__TYPE_DEF_H_
#define HUFFMAN__TYPE_DEF_H_
#include <cstddef>
#include <cstdint>


#define MAX_BIT 7u
#define BYTE 8u
#define FULL_BYTE 255u
typedef uint8_t ElemType;
static const size_t ALPHABET_SIZE = 256;
struct symbol {
    ElemType ch;
    size_t cnt;
    friend bool operator<(symbol const& a, symbol const& b) {
        return a.cnt < b.cnt;
    }
};


#endif //HUFFMAN__TYPE_DEF_H_
