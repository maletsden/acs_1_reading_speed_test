#include <iostream>
#include <deque>
#include <fstream>
#include <sstream>
#include <limits>
#include <string>


std::string read_file_into_string(std::fstream& in) ;
std::string read_file_into_string_by_file_iter(std::fstream& in);
std::string read_stream_into_string(std::fstream& in);
std::string read_file_ignore(std::fstream& in);
std::deque<std::string> read_file_into_deque(std::fstream& in);
