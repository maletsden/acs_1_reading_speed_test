#include <iostream>
#include <regex>
#include <string>
#include <boost/algorithm/string.hpp>

#include "words_count.h"

uint64_t num_not_ws(const std::string &container) {
    uint64_t num = 0;
    for (char chr : container) {
        if (!isspace(chr)) {
            ++num;
        }
    }
    return num;
}


uint64_t num_not_ws(const std::deque<std::string>& mydeque) {
    uint64_t num = 0;
    for (auto & it : mydeque) {
        num += num_not_ws(it);
    }
    return num;
}


uint64_t num_not_ws(const std::deque<char>& mydeque) {
    uint64_t num = 0;
    for (auto & it : mydeque) {
        if (!isspace(it)) {
            ++num;
        }
    }
    return num;
}

u_int64_t num_not_ws_regex(const std::string &container) {
    uint64_t num = 0;
    std::regex myregex ("\\S");

    // finding all the match.
    for (std::sregex_iterator it = std::sregex_iterator(container.begin(), container.end(), myregex);
         it != std::sregex_iterator(); it++)
        ++num;

    return num;
}

u_int64_t num_not_ws_split(const std::string &container) {
    std::vector<std::string> results;
    uint64_t num = 0;

    boost::split(results, container, [&num](char c) {
        if (!isspace(c)) {
            ++num;
        }
        return false;
    });

    // allocate memory
    std::vector<std::string>().swap(results);

    return num;
}


