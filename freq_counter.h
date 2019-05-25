//
// Created by taras on 25.05.19.
//

#ifndef HUFFMAN__FREQ_COUNTER_H_
#define HUFFMAN__FREQ_COUNTER_H_
#include <array>
#include <algorithm>
#include "type_def.h"

class freq_counter {
  private:
    std::array<symbol, ALPHABET_SIZE> data_{};

  public:
    freq_counter();
    ~freq_counter() = default;

    std::array<symbol, ALPHABET_SIZE> get_counter() {
        return data_;
    }

    template <typename InputIt>
    void update(InputIt first, InputIt last) {
        if (!std::is_trivially_copyable<typename std::iterator_traits<InputIt>::value_type>::value) {
            throw std::runtime_error("not trivially copyable type is prohibited");
        }
        while (first != last) {
            data_[*first].cnt++;
        }
    }

};

#endif //HUFFMAN__FREQ_COUNTER_H_
