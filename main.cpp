#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <cctype>
#include <string>
#include <sstream>

void print(const std::string &input) {
    for (char chr : input) {
        std::cout << chr;
    }

    std::cout << std::endl;
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


int num_not_ws(const std::vector<char> &container) {
    int num = 0;
    for (char chr : container) {
        if (!isspace(chr)) {
            ++num;
        }
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

    std::string chunk(chunk_size, '\0');

    while (
        in.read(&chunk[0], chunk_size) ||
        in.gcount()
    )
        container.insert(
            end(container),
            chunk
        );

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

    std::vector<char> container(char_count);

    if (!container.empty()) {
        if (!in.read(&container[0], container.size())){
            throw std::ios_base::failure{"error"};
        }
        std::cout << num_not_ws(container);
    }

    return container;
}

int main() {
    std::fstream in;
    in.open("test.txt", std::fstream::in);

    // First method
    std::string s = read_stream_into_string(in);
    print(s);

    // return file pointer to the beginning
    in.seekg (0, std::fstream::beg);

    // Second method
    std::vector<char> container = read_file_into_vector(in);

    // return file pointer to the beginning
    in.seekg (0, std::fstream::beg);

    // Third method
    std::deque<std::string> mydeque = read_file_into_deque(in);

    print_deque_of_strings(mydeque);

    in.close();
    return 0;
}

