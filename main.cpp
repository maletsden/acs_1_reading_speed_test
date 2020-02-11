#include <iostream>
#include <fstream>
#include <limits>
#include <vector>
#include <cctype>

void print(std::vector<char> const &input)
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


int num_not_ws(std::vector<char> const &container){
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

    std::vector<char> container(char_count);

    if (0 != container.size())
    {
        if (!in.read(&container[0], container.size())){
            throw std::ios_base::failure{"error"};
        }
        std::cout << num_not_ws(container);
    }

    return container;
}

int main() {
    std::fstream in;
    in.open("test_small.txt", std::fstream::in);

    std::vector<char> container = read_file_into_memory(in);


    in.close();
    return 0;
}

