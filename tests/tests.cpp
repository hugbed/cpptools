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

TEST(probe_count, count_is_correct)
{
    probe_count::resetCount();
    probe_count a{};
    const probe_count b{a};
    probe_count c{b};
    probe_count d{std::move(a)};

    EXPECT_EQ(probe_count::baseConstructionCount, 1);
    EXPECT_EQ(probe_count::copyConstructionCount, 1);
    EXPECT_EQ(probe_count::crefConstructionCount, 1);
    EXPECT_EQ(probe_count::moveConstructionCount, 1);
}

TEST(producer, move_construction)
{
    probe_count::resetCount();
    Producer<probe_count> producer;

    auto t = std::thread([&producer]() {
      probe_count in_probe;
      producer.produce(std::move(in_probe));
    });
    t.join();

    // consume
    auto out_probe = producer.consume();

    EXPECT_EQ(probe_count::baseConstructionCount, 1);
    EXPECT_EQ(probe_count::copyConstructionCount, 0);
    EXPECT_EQ(probe_count::crefConstructionCount, 0);
    EXPECT_EQ(probe_count::moveConstructionCount, 2);
}

TEST(producer, copy_construction)
{
    probe_count::resetCount();
    Producer<probe_count> producer;

    auto t = std::thread([&producer]() {
      probe_count in_probe;
      producer.produce(in_probe);
    });
    t.join();

    // consume
    auto out_probe = producer.consume();

    EXPECT_EQ(probe_count::baseConstructionCount, 1);
    EXPECT_EQ(probe_count::copyConstructionCount, 0);
    EXPECT_EQ(probe_count::crefConstructionCount, 1);
    EXPECT_EQ(probe_count::moveConstructionCount, 1);
}


TEST(ifchunkstream, file_reading)
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
