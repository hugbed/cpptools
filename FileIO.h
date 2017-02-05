//
// Created by Hugo Bedard on 17-02-04.
//

#ifndef FILEREAD_FILEIO_H
#define FILEREAD_FILEIO_H

#include <stddef.h>
#include <fstream>
#include <vector>

namespace fileio {
using byte_t = unsigned char;

template<class Size_t>
static void push_back_n_bytes(std::ifstream& stream, Size_t n, std::vector<byte_t>& v)
{
    std::copy_n(std::istreambuf_iterator<char>{stream},
            n,
            back_inserter(v));
}

std::vector<byte_t> load_bytes(const std::string& filename)
{
    auto bytes = std::vector<byte_t>{};
    std::ifstream in{filename, std::ios::binary};
    std::copy(std::istreambuf_iterator<char>{in},
            std::istreambuf_iterator<char>{},
            back_inserter(bytes));
    return bytes;
}

template<class Container>
bool write_bytes(const std::string& filename, Container c)
{
    std::ofstream out{filename, std::ios::binary};
    if (out.is_open()) {
        std::copy(std::begin(c), std::end(c), std::ostreambuf_iterator<char>{out});
        return true;
    }
    return false;
}

template<class Size_t>
std::vector<byte_t> load_n_bytes(const std::string& filename, Size_t n)
{
    auto bytes = std::vector<byte_t>{};
    std::ifstream in{filename, std::ios::binary};
    std::copy_n(std::istreambuf_iterator<char>{in},
            n,
            back_inserter(bytes));
    return bytes;
}

class ifchunkstream {
public:
    ifchunkstream(std::string filename, size_t chunkSize)
            :stream_{filename, std::ios::binary}, chunkSize_{chunkSize}
    {
    }

    friend ifchunkstream& operator>>(ifchunkstream& input, std::vector<byte_t>& v)
    {
        input.push_back_chunk(v);
        return input;
    }

private:
    void push_back_chunk(std::vector<byte_t>& v)
    {
        push_back_n_bytes(stream_, chunkSize_, v);
    }

    size_t chunkSize_;
    std::ifstream stream_;
};
}

#endif //FILEREAD_FILEIO_H
