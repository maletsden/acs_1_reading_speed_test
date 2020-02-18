#include "file_readers.h"


uint64_t read_file_into_string(std::fstream& in){
    std::string s;
    char c;
    uint64_t num_c = 0;
    while( in >> std::noskipws >> c ){
        if (!isspace(c)){
            num_c++;
        }
        s.push_back(c);
    }

    s.erase();

    // returning the number of non-whitespaces
    return num_c;
}

std::string read_stream_into_string(std::fstream& in) {
    std::ostringstream ss;

    if (!(ss << in.rdbuf()))
        throw std::ios_base::failure{"error"};

    return ss.str();
}

std::deque<std::string> read_file_into_deque(std::fstream& in) {
    auto const chunk_size = std::size_t{BUFSIZ};

    std::deque<std::string> container;

    std::string chunk;
    chunk.resize(chunk_size);

    while (
            in.read(&chunk[0], chunk_size) ||
            in.gcount()
            )
        container.insert(
                end(container),
                in.gcount() != chunk_size ? chunk.substr(0, in.gcount()) : chunk
        );

    return container;
}

std::string read_file_ignore(std::fstream& in) {
    auto const start_pos = in.tellg();
    if (std::streamsize(-1) == start_pos) {
        throw std::ios_base::failure{"error1"};
    }

    if (!in.ignore(std::numeric_limits<std::streamsize>::max())) {
        throw std::ios_base::failure{"error2"};
    }

    auto const char_count = in.gcount();

    if (!in.seekg(start_pos)){
        throw std::ios_base::failure{"error3"};
    }

    std::string container;
    container.resize(char_count);

    if (!container.empty()) {
        if (!in.read(&container[0], container.size())) {
            throw std::ios_base::failure{"error4"};
        }
    }

    return container;
}

