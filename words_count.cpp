#include <iostream>

#include "words_count.h"

uint64_t num_not_ws(const std::string &container){
    uint64_t num = 0;
    for (char chr : container) {
        if (!isspace(chr)) {
            ++num;
        }
    }
    return num;
}

uint64_t num_not_ws(const std::deque<std::string>& mydeque){
    uint64_t num = 0;
    for (auto & it : mydeque) {
        num += num_not_ws(it);
    }
    return num;
}
