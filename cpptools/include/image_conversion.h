//
// Created by Hugo Bedard on 17-02-04.
//

#ifndef FILEREAD_IMAGECONVERSION_H
#define FILEREAD_IMAGECONVERSION_H

#include <vector>
#include "math_algorithm.h"
#include "types.h"

namespace hb {

namespace image_format {
    struct YUV8_422 { };
    struct RGB888 { };
}

template <class T>
struct Dimensions_base {
    T width;
    T height;
};
using Dimensions = Dimensions_base<size_t>;

template<class YUV8_422, class RGB888>
std::tuple<byte_t, byte_t, byte_t> color_cast(byte_t Y, byte_t U, byte_t V)
{
    int C = Y-16;
    int D = U-128;
    int E = V-128;

    auto R = static_cast<byte_t>(clamp((298*C+409*E+128) >> 8, 0, 255));
    auto G = static_cast<byte_t>(clamp((298*C-100*D-208*E+128) >> 8, 0, 255));
    auto B = static_cast<byte_t>(clamp((298*C+516*D+128) >> 8, 0, 255));

    return std::tuple<byte_t, byte_t, byte_t>{R, G, B};
}

template<class YUV8_422, class RGB888>
static std::vector<byte_t> color_cast(const std::vector<byte_t>& in, Dimensions imageSize)
{
    // 4:2:2
    constexpr int samplingRatio = 2;

    // naive serial 4:2:2 to rgb
    auto width = imageSize.width;
    auto height = imageSize.height;
    auto imageRGBA = std::vector<byte_t>(width*height*samplingRatio*2, 0);

    byte_t y0, cb0, y1, cr0;
    for (int i = 0; i < height*width*samplingRatio; i += 4) {
        y1 = in[i+3];
        cr0 = in[i+2];
        y0 = in[i+1];
        cb0 = in[i+0];

        byte_t R, G, B;
        std::tie(R, G, B) = color_cast<YUV8_422, RGB888>(y0, cb0, cr0);

        // little endian
        const int argbIndex = i*2;
        imageRGBA[argbIndex+3] = 255;
        imageRGBA[argbIndex+2] = R;
        imageRGBA[argbIndex+1] = G;
        imageRGBA[argbIndex+0] = B;

        std::tie(R, G, B) = color_cast<YUV8_422, RGB888>(y1, cb0, cr0);
        imageRGBA[argbIndex+7] = 255;
        imageRGBA[argbIndex+6] = R;
        imageRGBA[argbIndex+5] = G;
        imageRGBA[argbIndex+4] = B;
    }

    return imageRGBA;
}

} // namespace hb

#endif //FILEREAD_IMAGECONVERSION_H
