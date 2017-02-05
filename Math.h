//
// Created by Hugo Bedard on 17-02-04.
//

#ifndef FILEREAD_MATH_H
#define FILEREAD_MATH_H

#include <algorithm>

namespace math {

template<typename T>
T clamp(const T& v, const T& lo, const T& hi)
{
    return std::min(std::max(v, lo), hi);
}

}

#endif //FILEREAD_MATH_H
