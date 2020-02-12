#include <iostream>
#include <fstream>
#include <limits>
#include <chrono>
#include <atomic>
#include <cctype>
#include <string>
#include <sstream>

void print(const std::string &input) {
    for (char chr : input) {
        std::cout << chr;
    }

    std::cout << std::endl;
}


inline std::chrono::high_resolution_clock::time_point get_current_time_fenced() {
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D& d) {
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

void print_deque_of_strings(const std::deque<std::string>& mydeque) {
    for (auto & it : mydeque) {
        print(it);
    }
}
void file_is_opened(const std::fstream& in) {
    if (!in.is_open()){
        throw std::ios_base::failure{"Error. File is closed."};
    }
}



uint64_t num_not_ws(const std::string &container){
    uint64_t num = 0;
    for (char chr : container) {
        if (!isspace(chr)) {
            ++num;
        }
    }
    return num;
}

uint64_t num_not_ws_in_deque(const std::deque<std::string>& mydeque){
    uint64_t num = 0;
    for (auto & it : mydeque) {
        num += num_not_ws(it);
    }
    return num;
}




std::string read_stream_into_string(const std::fstream& in) {
    file_is_opened(in);

    std::ostringstream ss;

    if (!(ss << in.rdbuf()))
        throw std::ios_base::failure{"error"};

    return ss.str();
}

std::deque<std::string> read_file_into_deque(std::fstream& in) {
    file_is_opened(in);

    auto const chunk_size = std::size_t{BUFSIZ};

    std::deque<std::string> container;

    std::string chunk;
    chunk.resize(chunk_size);

    while (
        in.read(&chunk[0], chunk_size) ||
        in.gcount()
    ) {
        if (in.gcount() != chunk_size) {
            chunk.resize(in.gcount());
        }

        container.insert(
            end(container),
            chunk
        );

        if (in.gcount() != chunk_size) {
            chunk.resize(chunk_size);
        }
    }
    return container;
}

auto read_file_into_vector(std::fstream& in) {
    file_is_opened(in);

    auto const start_pos = in.tellg();
    if (std::streamsize(-1) == start_pos) {
        throw std::ios_base::failure{"error"};
    }

    if (!in.ignore(std::numeric_limits<std::streamsize>::max())) {
        throw std::ios_base::failure{"error"};
    }

    auto const char_count = in.gcount();

    if (!in.seekg(start_pos)){
        throw std::ios_base::failure{"error"};
    }

    std::string container;
    container.resize(char_count);

    if (!container.empty()) {
        if (!in.read(&container[0], container.size())) {
            throw std::ios_base::failure{"error"};
        }
    }

    return container;
}

int main() {
    std::fstream in;
    in.open("test_file.txt", std::fstream::in);

    auto start = get_current_time_fenced();

    // First method
    std::string s = read_stream_into_string(in);

    auto first_end = get_current_time_fenced();

    // return file pointer to the beginning
    in.seekg (0, std::fstream::beg);

    auto second_start = get_current_time_fenced();

    // Second method
    std::string container = read_file_into_vector(in);

    auto second_end = get_current_time_fenced();

    // return file pointer to the beginning
    in.seekg (0, std::fstream::beg);

    auto third_start = get_current_time_fenced();

    // Third method
    std::deque<std::string> mydeque = read_file_into_deque(in);

    auto third_end = get_current_time_fenced();

    auto amount_of_nonwchars = num_not_ws(s);

    // check method results
    if (
        amount_of_nonwchars != num_not_ws(container) ||
        amount_of_nonwchars != num_not_ws_in_deque(mydeque)
    ) {
        throw std::ios_base::failure{"Methods results aren't equal."};
    }

    std::cout << "Amount of non-whitespaces chars in file: " << amount_of_nonwchars << std::endl << std::endl;

    std::cout << "First  method (in us): " << to_us(first_end - start) << std::endl;
    std::cout << "Second method (in us): " << to_us(second_end - second_start) << std::endl;
    std::cout << "Third  method (in us): " << to_us(third_end - third_start) << std::endl;
    in.close();
    return 0;
}

