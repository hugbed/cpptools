#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include "Queue.h"

using byte = char;

struct probe {
    probe() { std::cout << "probe() " << ((void*)this) << std::endl; }
    probe(const probe&) { std::cout << "probe(c&) " << ((void*)this) << std::endl; }
    probe(probe&&) { std::cout << "probe(&&) " << ((void*)this) << std::endl; }
    ~probe() { std::cout << "~probe() " << ((void*)this) << std::endl; }
    probe& operator=(const probe&) { std::cout << "probe=(c&) " << ((void*)this) << std::endl;  }
    probe& operator=(probe&&) { std::cout << "probe=(&&) " << ((void*)this) << std::endl;  }
};

class ifchunkstream {
public:
    ifchunkstream(std::string filename, size_t chunkSize)
    : stream_{filename, std::ios::binary}
    , chunkSize_{ chunkSize }
    {
    }

    friend ifchunkstream &operator>>(ifchunkstream &input, std::vector<byte> &v) {
        input.push_back_chunk(v);
        return input;
    }

private:
    void push_back_chunk(std::vector<byte> &v)
    {
        std::copy_n(std::istreambuf_iterator<char>{stream_},
                    chunkSize_,
                    back_inserter(v));
    }

    size_t chunkSize_;
    std::ifstream stream_;
};

template <class T>
class Producer {
public:
    using value_type = T;

    Producer() = default;

    T consume()
    {
        return queue_.pop();
    }

    void produce(const T& t)
    {
        queue_.push(t);
    }

    void produce(T&& t) {
        queue_.push(std::forward<T>(t));
    }

private:
    Queue<T> queue_;
};

template <class Container>
void println_vals(Container c) {
    for (auto val: c) {
        std::cout << val;
    }
    std::cout << std::endl;
}

int main() {
    // test move construction
    {
        Producer<probe> producer;

        auto t = std::thread([&producer]() {
            probe in_bytes;
            producer.produce(std::move(in_bytes));
        });
        t.join();

        // consume
        auto return_probe = producer.consume();
    }

    // test byte reading
    {
        Producer<std::vector<byte>> producer;

        auto t = std::thread([&producer]() {
            ifchunkstream cs{"/Users/hugbed/ClionProjects/FileRead/data.bin", 128};

            std::vector<byte> in_bytes;
            cs >> in_bytes;
            producer.produce(std::move(in_bytes));

            in_bytes.clear();
            cs >> in_bytes;
            producer.produce(std::move(in_bytes));
        });
        t.join();

        // consume
        std::vector<byte> out_bytes;
        out_bytes = producer.consume();
        println_vals(out_bytes);

        out_bytes = producer.consume();
        println_vals(out_bytes);
    }
}
