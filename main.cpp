#include <iostream>
#include <fstream>
#include <chrono>
#include <atomic>
#include <cctype>
#include <string>
#include <cstring>

#include "words_count.h"
#include "file_readers.h"


#define REQUIRED_ARGS_AMOUNT    4
#define METHOD_TYPE_ARG         1
#define IN_FILE_ARG             2
#define OUT_FILE_ARG            3

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



int main(int argc, char** argv) {
    if (argc != REQUIRED_ARGS_AMOUNT) {
        throw std::ios_base::failure{"Wrong amount of arguments (expected - 4)."};
    }

    if (
            strlen(argv[METHOD_TYPE_ARG])  != 1 &&
            !isdigit(argv[METHOD_TYPE_ARG][0])
            ) {
        throw std::ios_base::failure{
                "No supported method with selected type."
        };
    }

    std::fstream in;
    in.open(argv[IN_FILE_ARG], std::fstream::in);

    // check existence of input file
    if (!in.is_open()){
        throw std::ios_base::failure{
                "Selected file is closed or doesn't exist."
        };
    }

    std::cout << "Reading the entire file into a memory using method of ";

    uint64_t amount_of_non_wspases = 0;
    std::chrono::high_resolution_clock::time_point start, end;

    switch (std::stoi(argv[METHOD_TYPE_ARG])) {
        case 1: {
            std::cout << "reading stream into string (in us): ";

            start = get_current_time_fenced();
            auto container = read_stream_into_string(in);
            amount_of_non_wspases = num_not_ws(container);

            break;
        }

        case 2: {
            std::cout << "reading chunks of file into deque (in us): ";

            start = get_current_time_fenced();
            auto container = read_file_into_deque(in);
            amount_of_non_wspases = num_not_ws(container);

            break;
        }

        case 3: {
            std::cout << "'ignoring' file and reading all of it afterwards (in us): ";

            start = get_current_time_fenced();
            auto container = read_file_ignore(in);
            amount_of_non_wspases = num_not_ws(container);

            break;
        }
        case 4: {
            std::cout << "reading file char by char and appending to string (in us): ";

            start = get_current_time_fenced();
            amount_of_non_wspases  = read_file_into_string(in);

            break;
        }
    }

    end = get_current_time_fenced();

    std::cout << std::endl;

    // print time of operation
    std::cout << to_us(end - start) << std::endl;

    in.close();

    // save results
    std::fstream out;

    out.open(argv[OUT_FILE_ARG], std::fstream::out);

    out << "There were read " << amount_of_non_wspases << " of non-whitespace characters from file: " <<
        argv[IN_FILE_ARG] << std::endl;

    out.close();

    return 0;
}

