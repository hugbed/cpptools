//
// Created by Hugo Bedard on 2017-02-04.
//

#ifndef FILEREAD_TESTS_H
#define FILEREAD_TESTS_H


#include <iostream>
#include <vector>
#include <fstream>
#include <thread>
#include <assert.h>
#include <chrono>

#include "queue.h"
#include "file_io.h"
#include "utils.h"
#include "producer.h"
#include "image_conversion.h"

#include "gtest/gtest.h"

using namespace hb;

TEST(producer, move_construction)
{
    // TODO: generalize probe to use functor instead of std::cout <<
    Producer<probe> producer;

    auto t = std::thread([&producer]() {
      probe in_bytes;
      producer.produce(std::move(in_bytes));
    });
    t.join();

    // consume
    auto return_probe = producer.consume();

    EXPECT_EQ(0, 0);
}

TEST(producer, file_reading)
{
    //TODO: mock file reading?
    Producer<std::vector<byte_t>> producer;

    auto t = std::thread([&producer]() {
      ifchunkstream cs{"/Users/hugbed/ClionProjects/cpptools/tests/data/data.bin", 128};

      std::vector<byte_t> in_bytes;
      cs >> in_bytes;
      producer.produce(std::move(in_bytes));

      in_bytes.clear();
      cs >> in_bytes;
      producer.produce(std::move(in_bytes));
    });
    t.join();

    // consume
    std::vector<byte_t> out_bytes;
    out_bytes = producer.consume();
    println(out_bytes);

    out_bytes = producer.consume();
    println(out_bytes);

    EXPECT_EQ(0, 0);
}

void dummy_function()
{
    for (int i = 0; i < 10; i++) {
        int j = 0;
    }
}

TEST(mesure_time_wrapper, returns_time)
{
    using namespace std::chrono;
    auto dt = mesure_time([](){
      dummy_function();
    });
    auto dt_ns = duration_cast<nanoseconds>(dt);
    EXPECT_GT(dt_ns.count(), 0);
}

TEST(image_conversion, YUV8_422_to_RGB888)
{
    //TODO: do with dummy pixels (e.g., only R, only G, only B)
    std::vector<byte_t> image = load_bytes("/Users/hugbed/ClionProjects/cpptools/tests/data/image.yuv");
    EXPECT_GT(image.size(), 0);

    using namespace image_format;
    image = color_cast<YUV8_422, RGB888>(image);
    EXPECT_GT(image.size(), 0);

    write_bytes("/Users/hugbed/ClionProjects/cpptools/tests/data/image.raw", image);
}


#endif //FILEREAD_TESTS_H
