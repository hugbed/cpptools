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
    using probe = probe<probe_policy::count>;

    probe::resetCount();
    probe a{};
    const probe b{a};
    probe c{b};
    probe d{std::move(a)};

    EXPECT_EQ(probe::baseConstructionCount, 1);
    EXPECT_EQ(probe::copyConstructionCount, 0);
    EXPECT_EQ(probe::crefConstructionCount, 2);
    EXPECT_EQ(probe::moveConstructionCount, 1);
}

TEST(producer, move_construction)
{
    using probe = probe<probe_policy::count>;

    probe::resetCount();
    Producer<probe> producer;

    auto t = std::thread([&producer]() {
      probe in_probe;
      producer.produce(std::move(in_probe));
    });
    t.join();

    // consume
    auto out_probe = producer.consume();

    EXPECT_EQ(probe::baseConstructionCount, 1);
    EXPECT_EQ(probe::copyConstructionCount, 0);
    EXPECT_EQ(probe::crefConstructionCount, 0);
    EXPECT_EQ(probe::moveConstructionCount, 2);
}

TEST(producer, cref_construction)
{
    using probe = probe<probe_policy::count>;

    probe::resetCount();
    Producer<probe> producer;

    auto t = std::thread([&producer]() {
      probe in_probe;
      producer.produce(in_probe);
    });
    t.join();

    // consume
    auto out_probe = producer.consume();

    EXPECT_EQ(probe::baseConstructionCount, 1);
    EXPECT_EQ(probe::copyConstructionCount, 0);
    EXPECT_EQ(probe::crefConstructionCount, 1);
    EXPECT_EQ(probe::moveConstructionCount, 1);
}

TEST(ifchunkstream, file_reading)
{
    std::cout << std::endl;

    std::vector<char> gold_first, gold_second;
    gold_first.resize(128); gold_second.resize(128);
    std::fill(std::begin(gold_first), std::end(gold_first), 'A');
    std::fill(std::begin(gold_second), std::end(gold_second), 'B');

    ifchunkstream cs{"data/data.bin", 128};
    std::vector<byte_t> file_bytes;
    cs >> file_bytes;
    println(gold_first);
    println(file_bytes);
    EXPECT_EQ(file_bytes.size(), 128);
    EXPECT_EQ(true,
            std::equal(std::begin(gold_first),
                    std::end(gold_first),
                    std::begin(file_bytes)));

    file_bytes.clear();
    cs >> file_bytes;
    println(gold_second);
    println(file_bytes);
    EXPECT_EQ(file_bytes.size(), 128);
    EXPECT_EQ(true,
            std::equal(std::begin(gold_second),
                    std::end(gold_second),
                    std::begin(file_bytes)));
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
