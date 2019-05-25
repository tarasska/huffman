//
// Created by taras on 25.05.19.
//

#include "freq_counter.h"
freq_counter::freq_counter() {
    for (size_t i = 0; i < ALPHABET_SIZE; ++i) {
        data_[i] = {static_cast<ElemType>(i), 0};
    }
}
