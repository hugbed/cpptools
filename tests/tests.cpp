//
// Created by Hugo Bedard on 2017-02-04.
//

#ifndef FILEREAD_TESTS_H
#define FILEREAD_TESTS_H


#include <iostream>
#include <vector>
#include <fstream>
#include <thread>

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

// last byte should not be repeated, e.g., ABBC instead of ABCD
TEST(ifchunkstream, file_reading_multiple_chunks)
{
    std::cout << std::endl;

    std::vector<byte_t> gold{'A', 'B', 'C', 'D'};

    std::vector<byte_t> b;
    ifchunkstream cs{"data/data.bin", 2};
    b.clear();
    cs >> b;
    cs >> b;
    println(b);
    EXPECT_EQ(true,
            std::equal(std::begin(gold),
                    std::end(gold),
                    std::begin(b)));
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

// TODO: Put more thoughts into these two tests
TEST(image_conversion, YUV8_422_to_RGB888_All_0)
{
    using namespace image_format;

    std::vector<byte_t> gold = {
            0, 135, 0, 255,
            0, 135, 0, 255
        };

    std::vector<byte_t> imageYUV = {0, 0, 0, 0}; // two black pixels
    auto resRGB = color_cast<YUV8_422, RGB888>(imageYUV, Dimensions{2, 1});

    EXPECT_EQ(true,
              std::equal(std::begin(gold),
                         std::end(gold),
                         std::begin(resRGB)));
}

TEST(image_conversion, YUV8_422_to_RGB888_All_255)
{
    using namespace image_format;

    std::vector<byte_t> gold = {
            255, 125, 255, 255,
            255, 125, 255, 255
        };

    std::vector<byte_t> imageYUV = {255, 255, 255, 255}; // two black pixels
    auto resRGB = color_cast<YUV8_422, RGB888>(imageYUV, Dimensions{2, 1});

    EXPECT_EQ(true,
              std::equal(std::begin(gold),
                         std::end(gold),
                         std::begin(resRGB)));
}

#endif //FILEREAD_TESTS_H
