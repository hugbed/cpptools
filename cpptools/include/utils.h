//
// Created by Hugo Bedard on 17-02-04.
//

#ifndef FILEREAD_UTILS_H
#define FILEREAD_UTILS_H

#include <iostream>
#include <chrono>

namespace hb {

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

template <typename Predicate, typename... Params>
auto mesure_time(Predicate f, Params&&... params)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    f(std::forward<Params>(params)...);
    auto t2 = std::chrono::high_resolution_clock::now();
    return t2 - t1;
}

} // namespace hb

#endif //FILEREAD_UTILS_H
