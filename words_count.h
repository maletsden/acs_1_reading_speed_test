#include <iostream>
#include <deque>
#include <boost/algorithm/string.hpp>

uint64_t num_not_ws(const std::string &container);
uint64_t num_not_ws(const std::deque<std::string>& mydeque);
uint64_t num_not_ws(const std::deque<char>& mydeque);
u_int64_t num_not_ws_regex(const std::string &container);
u_int64_t num_not_ws_split(const std::string &container);
