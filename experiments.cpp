#include <iostream>
#include <fstream>
#include <chrono>
#include <atomic>
#include <cctype>
#include <string>
#include <limits>
#include <sstream>
#include <climits>

#include "file_readers.h"
#include "words_count.h"

#define REQUIRED_ARGS_AMOUNT    4
#define AMOUNT_OF_EXPERIMENTS   1
#define IN_FILE_ARG             2
#define OUT_FILE_ARG            3

#define NUM_OF_METHODS          7

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

#define PBSTR       "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH     60

void printProgress (double percentage) {
    int val = (int) (percentage * 100);
    int lpad = (int) (percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}


typedef std::string (*fileReadersPointer)(std::fstream&);
std::chrono::high_resolution_clock::time_point start, end;

long long run_method(std::fstream& in, fileReadersPointer func, uint64_t expected_number) {
    start = get_current_time_fenced();
    std::string s_container = read_stream_into_string(in);
    if (num_not_ws(s_container) != expected_number) {
        throw std::ios_base::failure{
                "Experiments gave different results."
        };
    }

    end = get_current_time_fenced();

    // return file pointer to the beginning
    in.clear();
    in.seekg (0, std::fstream::beg);

    return to_us(end - start);
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
    if (!in.is_open()) {
        std::cout << argv[IN_FILE_ARG] << std::endl;
        throw std::ios_base::failure{
                "Selected file is closed or doesn't exist."
        };
    }

    
    uint64_t num_of_non_spaces, read_num_of_non_spaces;
    std::string s_container;
    std::deque<std::string> d_container;
    long long exp1_time = LLONG_MAX,
            exp2_time = LLONG_MAX,
            exp3_time = LLONG_MAX,
            exp4_time = LLONG_MAX,
            exp5_time = LLONG_MAX,
            exp6_time = LLONG_MAX,
            exp7_time = LLONG_MAX;



    // first read for future checking equivalence of results
    num_of_non_spaces = num_not_ws(
        read_stream_into_string(in)
    );

    // return file pointer to the beginning
    in.clear();
    in.seekg (0, std::fstream::beg);


    fileReadersPointer functionPointers[] = {
            read_file_into_string,
            read_file_into_string_by_file_iter,
            read_stream_into_string,
            read_file_ignore
    };

    // run experiments
    for (int i = 0; i < num_of_experiments; ++i) {
        // print loading status
        printProgress(((double) i * NUM_OF_METHODS ) / (num_of_experiments * NUM_OF_METHODS));

        // First method
        exp1_time = std::min(
                exp1_time,
                run_method(in, functionPointers[0], num_of_non_spaces)
        );

        // print loading status
        printProgress(((double) i * NUM_OF_METHODS  + 1) / (num_of_experiments * NUM_OF_METHODS));

        // Second method
        exp2_time = std::min(
                exp2_time,
                run_method(in, functionPointers[1], num_of_non_spaces)
        );

        // print loading status
        printProgress(((double) i * NUM_OF_METHODS  + 2) / (num_of_experiments * NUM_OF_METHODS));

        // Third method
        exp3_time = std::min(
                exp3_time,
                run_method(in, functionPointers[2], num_of_non_spaces)
        );

        // print loading status
        printProgress(((double) i * NUM_OF_METHODS  + 3) / (num_of_experiments * NUM_OF_METHODS));

        // Forth method
        exp4_time = std::min(
                exp4_time,
                run_method(in, functionPointers[3], num_of_non_spaces)
        );

        // print loading status
        printProgress(((double) i * NUM_OF_METHODS  + 4) / (num_of_experiments * NUM_OF_METHODS));

        // Fifth method
        start = get_current_time_fenced();
        d_container = read_file_into_deque(in);
        if (num_not_ws(d_container) != num_of_non_spaces) {
            throw std::ios_base::failure{
                    "Experiments gave different results."
            };
        }
        end = get_current_time_fenced();
        exp5_time = std::min(exp5_time, to_us(end - start));

        // return file pointer to the beginning
        in.clear();
        in.seekg (0, std::fstream::beg);


        // print loading status
        printProgress(((double) i * NUM_OF_METHODS  + 5) / (num_of_experiments * NUM_OF_METHODS));

        // Sixth method

        start = get_current_time_fenced();
        s_container = read_file_ignore(in);
        if (num_not_ws_regex(s_container) != num_of_non_spaces) {
            throw std::ios_base::failure{
                    "Experiments gave different results."
            };
        }
        end = get_current_time_fenced();
        exp6_time = std::min(exp6_time, to_us(end - start));

        // return file pointer to the beginning
        in.clear();
        in.seekg (0, std::fstream::beg);

        // print loading status
        printProgress(((double) i * NUM_OF_METHODS  + 6) / (num_of_experiments * NUM_OF_METHODS));


        // Seventh method

        start = get_current_time_fenced();
        s_container = read_file_ignore(in);
        if (num_not_ws_split(s_container) != num_of_non_spaces) {
            throw std::ios_base::failure{
                    "Experiments gave different results."
            };
        }
        end = get_current_time_fenced();
        exp7_time = std::min(exp7_time, to_us(end - start));

        // return file pointer to the beginning
        in.clear();
        in.seekg (0, std::fstream::beg);
    }

    // print loading status
    printProgress(1.0);

    s_container.clear();
    d_container.clear();
    in.close();

    // print results
    printf("\rReading the entire file into a memory and calculating non-whitespaces (in us):\r\n\r\n");
    fflush(stdout);


    std::cout << "Stream into string:                  " << exp1_time << std::endl;
    std::cout << "Ignoring/reading:                    " << exp2_time << std::endl;
    std::cout << "Char by char into string:            " << exp3_time << std::endl;
    std::cout << "Reading using file iterator:         " << exp4_time << std::endl;
    std::cout << "Chunks into deque:                   " << exp5_time << std::endl;
    std::cout << "Ignoring/reading (count by regex):   " << exp6_time << std::endl;
    std::cout << "Ignoring/reading (count by split):   " << exp7_time << std::endl;


    std::cout << "\r\nAll results are the same." << std::endl;


    // save results
    std::fstream out;

    out.open(argv[OUT_FILE_ARG], std::fstream::out);

    out << "There were read " << num_of_non_spaces << " of non-whitespace characters from file: " <<
        argv[IN_FILE_ARG] << std::endl;

    out.close();

    return 0;
}