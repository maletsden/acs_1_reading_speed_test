#include <iostream>
#include <fstream>
#include <chrono>
#include <atomic>
#include <cctype>
#include <string>
#include <limits>
#include <sstream>

#include "file_readers.h"
#include "words_count.h"

#define REQUIRED_ARGS_AMOUNT    4
#define AMOUNT_OF_EXPERIMENTS   1
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

#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void printProgress (double percentage)
{
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf ("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush (stdout);
}
int main(int argc, char** argv) {
    if (argc != REQUIRED_ARGS_AMOUNT) {
        throw std::ios_base::failure{"Wrong amount of arguments (expected - 4)."};
    }

    int num_of_experiments;
    try {
        num_of_experiments = std::stoi(argv[AMOUNT_OF_EXPERIMENTS]);

        if (num_of_experiments < 0) {
            throw std::ios_base::failure{""};
        }
    } catch ( ... ) {
        throw std::ios_base::failure{
            "Amount of experiments must be a positive integer."
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



    uint64_t num_of_non_spaces, read_num_of_non_spaces;
    std::chrono::high_resolution_clock::time_point start, end;
    std::string s_container;
    std::deque<std::string> d_container;
    long long exp1_time = LLONG_MAX,
            exp2_time = LLONG_MAX,
            exp3_time = LLONG_MAX;

    // first read for future checking equivalence of results
    num_of_non_spaces = num_not_ws(
        read_stream_into_string(in)
    );

    // return file pointer to the beginning
    in.clear();
    in.seekg (0, std::fstream::beg);


    // just for visualization of loading procces

    // run experiments
    for (int i = 0; i < num_of_experiments; ++i) {
        // print loading status
        printProgress(((double) i ) / num_of_experiments);


        // First method

        start = get_current_time_fenced();
        s_container = read_stream_into_string(in);
        read_num_of_non_spaces = num_not_ws(s_container);
        if (read_num_of_non_spaces != num_of_non_spaces) {
            throw std::ios_base::failure{
                "Experiments gave different results."
            };
        }
        end = get_current_time_fenced();
        exp1_time = std::min(exp1_time, to_us(end - start));

        // return file pointer to the beginning
        in.clear();
        in.seekg (0, std::fstream::beg);



        // Second method

        start = get_current_time_fenced();
        d_container = read_file_into_deque(in);
        read_num_of_non_spaces = num_not_ws(d_container);
        if (read_num_of_non_spaces != num_of_non_spaces) {
            throw std::ios_base::failure{
                "Experiments gave different results."
            };
        }
        end = get_current_time_fenced();
        exp2_time = std::min(exp2_time, to_us(end - start));

        // return file pointer to the beginning
        in.clear();
        in.seekg (0, std::fstream::beg);



        // Third method

        start = get_current_time_fenced();
        s_container = read_file_into_string(in);
        read_num_of_non_spaces = num_not_ws(s_container);
        if (read_num_of_non_spaces != num_of_non_spaces) {
            throw std::ios_base::failure{
                "Experiments gave different results."
            };
        }
        end = get_current_time_fenced();
        exp3_time = std::min(exp3_time, to_us(end - start));

        // return file pointer to the beginning
        in.clear();
        in.seekg (0, std::fstream::beg);
    }

    // print loading status
    printProgress(1.0);

    in.close();

    // print results
    printf("\rReading the entire file into a memory and calculating non-whitespaces (in us):\r\n\r\n");
    fflush(stdout);


    std::cout << "Stream into string: " << exp1_time << std::endl;
    std::cout << "Chunks into deque:  " << exp2_time << std::endl;
    std::cout << "Ignoring/reading:   " << exp3_time << std::endl;

    std::cout << "\r\nAll results are the same." << std::endl;


    // save results
    std::fstream out;

    out.open(argv[OUT_FILE_ARG], std::fstream::out);

    out << "There were read " << num_of_non_spaces << " of non-whitespace characters from file: " <<
        argv[IN_FILE_ARG] << std::endl;

    out.close();

    return 0;
}