//
// Created by Hugo Bedard on 17-02-04.
//

#ifndef FILEREAD_UTILS_H
#define FILEREAD_UTILS_H

#include <iostream>

namespace utils {
struct probe {
  probe() { std::cout << "probe() " << ((void*) this) << std::endl; }
  probe(const probe&) { std::cout << "probe(c&) " << ((void*) this) << std::endl; }
  probe(probe&&) { std::cout << "probe(&&) " << ((void*) this) << std::endl; }
  ~probe() { std::cout << "~probe() " << ((void*) this) << std::endl; }
  probe& operator=(const probe&)
  {
      std::cout << "probe=(c&) " << ((void*) this) << std::endl;
      return *this;
  }
  probe& operator=(probe&&)
  {
      std::cout << "probe=(&&) " << ((void*) this) << std::endl;
      return *this;
  }
};

template<class Container>
void println(Container c)
{
    for (auto val: c) {
        std::cout << val;
    }
    std::cout << std::endl;
}
}

#endif //FILEREAD_UTILS_H
