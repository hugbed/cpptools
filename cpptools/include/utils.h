//
// Created by Hugo Bedard on 17-02-04.
//

#ifndef FILEREAD_UTILS_H
#define FILEREAD_UTILS_H

#include <iostream>
#include <chrono>

namespace hb {

struct probe {
  enum class FunctionCalled {
      BASE_CONSTRUCTION,
      COPY_CONSTRUCTION,
      CREF_CONSTRUCTION,
      MOVE_CONSTRUCTION,
      DESTRUCTION,
      COPY_ASSIGNATION,
      CREF_ASSIGNATION,
      MOVE_ASSIGNATION
  };

  probe() = delete;
  probe(const probe&) = delete;
  probe(probe&&) = delete;
};

struct probe_cout {
  probe_cout() { handler(probe::FunctionCalled::BASE_CONSTRUCTION); }
  probe_cout(probe_cout&) { handler(probe::FunctionCalled::COPY_CONSTRUCTION); }
  probe_cout(const probe_cout&) { handler(probe::FunctionCalled::CREF_CONSTRUCTION); }
  probe_cout(probe_cout&&) { handler(probe::FunctionCalled::MOVE_CONSTRUCTION); }
  ~probe_cout() { handler(probe::FunctionCalled::DESTRUCTION); }
  probe_cout& operator=(probe_cout) { handler(probe::FunctionCalled::COPY_ASSIGNATION); return *this; }
  probe_cout& operator=(const probe_cout&) { handler(probe::FunctionCalled::CREF_ASSIGNATION); return *this; }
  probe_cout& operator=(probe_cout&&) { handler(probe::FunctionCalled::MOVE_ASSIGNATION); return *this; }

  void handler(probe::FunctionCalled functionCalled)
  {
      switch (functionCalled) {
      case probe::FunctionCalled::BASE_CONSTRUCTION:
          printFunctionCalled("probe()");
          break;
      case probe::FunctionCalled::COPY_CONSTRUCTION:
          printFunctionCalled("probe(probe)");
          break;
      case probe::FunctionCalled::CREF_CONSTRUCTION:
          printFunctionCalled("probe(c&)");
          break;
      case probe::FunctionCalled::MOVE_CONSTRUCTION:
          printFunctionCalled("probe(&&)");
          break;
      case probe::FunctionCalled::DESTRUCTION:
          printFunctionCalled("~probe()");
          break;
      case probe::FunctionCalled::COPY_ASSIGNATION:
          printFunctionCalled("probe=()");
          break;
      case probe::FunctionCalled::CREF_ASSIGNATION:
          printFunctionCalled("probe=(c&)");
          break;
      case probe::FunctionCalled::MOVE_ASSIGNATION:
          printFunctionCalled("probe=(&&)");
          break;
      }
  }

  void printFunctionCalled(const std::string& functionCallStr)
  {
      std::cout << functionCallStr << " " << (void*)(this) << std::endl;
  }
};

struct probe_count {
  probe_count() { handler(probe::FunctionCalled::BASE_CONSTRUCTION); }
  probe_count(probe_count&) { handler(probe::FunctionCalled::COPY_CONSTRUCTION); }
  probe_count(const probe_count&) { handler(probe::FunctionCalled::CREF_CONSTRUCTION); }
  probe_count(probe_count&&) { handler(probe::FunctionCalled::MOVE_CONSTRUCTION); }

  void handler(probe::FunctionCalled f)
  {
      switch (f) {
      case probe::FunctionCalled::BASE_CONSTRUCTION:
          baseConstructionCount++;
          break;
      case probe::FunctionCalled::COPY_CONSTRUCTION:
          copyConstructionCount++;
          break;
      case probe::FunctionCalled::CREF_CONSTRUCTION:
          crefConstructionCount++;
          break;
      case probe::FunctionCalled::MOVE_CONSTRUCTION:
          moveConstructionCount++;
          break;
      default:
          // don't care about others
          break;
      }
  }

  static void resetCount()
  {
      probe_count::baseConstructionCount = 0;
      probe_count::copyConstructionCount = 0;
      probe_count::crefConstructionCount = 0;
      probe_count::moveConstructionCount = 0;
  }

  static size_t baseConstructionCount;
  static size_t copyConstructionCount;
  static size_t crefConstructionCount;
  static size_t moveConstructionCount;
};


template<class Container>
void println(Container c)
{
    for (auto val: c) {
        std::cout << val;
    }
    std::cout << std::endl;
}

template<typename Predicate, typename... Params>
auto mesure_time(Predicate f, Params&& ... params)
{
    auto t1 = std::chrono::high_resolution_clock::now();
    f(std::forward<Params>(params)...);
    auto t2 = std::chrono::high_resolution_clock::now();
    return t2 - t1;
}

} // namespace hb

#endif //FILEREAD_UTILS_H
