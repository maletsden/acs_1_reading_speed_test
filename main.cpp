#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <chrono>
#include <atomic>

inline std::chrono::high_resolution_clock::time_point get_current_time_fenced()
{
    std::atomic_thread_fence(std::memory_order_seq_cst);
    auto res_time = std::chrono::high_resolution_clock::now();
    std::atomic_thread_fence(std::memory_order_seq_cst);
    return res_time;
}

template<class D>
inline long long to_us(const D& d)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(d).count();
}

void print(std::string const &input)
{
    for (int i = 0; i < input.size(); i++) {
        std::cout << input.at(i);
    }
}

void file_is_opened(const std::fstream& in){
    if (!in.is_open()){
        throw std::ios_base::failure{"Error. File is closed."};
    }
}


int num_not_ws(std::string const &container){
    int num = 0;
    for (int i = 0; i < container.size(); ++i){
        if (!isspace(container.at(i))){
            ++num;
        }
    }
    return num;

}


auto read_file_into_vector(std::fstream& in)
{

    file_is_opened(in);

    auto const start_pos = in.tellg();
    if (std::streamsize(-1) == start_pos) {
        throw std::ios_base::failure{"error"};
    }

    if (!in.ignore(
            std::numeric_limits<std::streamsize>::max())){
        throw std::ios_base::failure{"error"};
    }
    auto const char_count = in.gcount();

    if (!in.seekg(start_pos)){
        throw std::ios_base::failure{"error"};
    }

    std::string container;
    container.resize(char_count);

    if (0 != container.size())
    {
        if (!in.read(&container[0], container.size())){
            throw std::ios_base::failure{"error"};
        }
        std::cout << num_not_ws(container) << std::endl;
    }

    return container;
}

int main() {


    std::fstream in;
    in.open("test_file.txt", std::fstream::in);

    auto start = get_current_time_fenced();

    std::string container = read_file_into_vector(in);

    auto end = get_current_time_fenced();
    auto total_time = end - start;
    std::cout << to_us(total_time) << std::endl;
    in.close();
    return 0;
}

