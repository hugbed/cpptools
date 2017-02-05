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
#include "Queue.h"
#include "fileio.h"
#include "utils.h"
#include "Producer.h"
#include "ImageConversion.h"

using utils::probe;
using utils::println;
using byte = fileio::byte_t;

void testProducerMoveConstruction()
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

void testFileReading()
{
    Producer<std::vector<byte>> producer;

    auto t = std::thread([&producer]() {
      fileio::ifchunkstream cs{"/Users/hugbed/ClionProjects/FileRead/data.bin", 128};

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
    println(out_bytes);

    out_bytes = producer.consume();
    println(out_bytes);
}

void testYUV8_422_to_RGB888()
{

    using namespace ImageConversion;

    std::vector<byte> image = fileio::load_bytes("/Users/hugbed/ClionProjects/FileRead/image.yuv");
    assert(image.size()>0);

    image = color_cast<YUV8_422, RGB888>(image);
    assert(image.size()>0);

    fileio::write_bytes("/Users/hugbed/ClionProjects/FileRead/image.raw", image);
}

#endif //FILEREAD_TESTS_H
