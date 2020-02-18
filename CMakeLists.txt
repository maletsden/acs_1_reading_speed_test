cmake_minimum_required(VERSION 3.12)
project(acs_1_reading_speed_test)

set(CMAKE_CXX_STANDARD 17)

add_executable(test_file_read main.cpp file_readers.cpp words_count.cpp)

add_executable(experiments experiments.cpp file_readers.cpp words_count.cpp)