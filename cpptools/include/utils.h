//
// Created by Hugo Bedard on 17-02-04.
//

#ifndef FILEREAD_UTILS_H
#define FILEREAD_UTILS_H

#include <iostream>
#include <chrono>
#include <map>

namespace hb {

namespace probe_policy {
    struct cout { };
    struct count{ };
}

template <typename T>
struct probe {
  enum class FunctionCalled {
      BASE_CONSTRUCTION,
      REF_CONSTRUCTION,
      CREF_CONSTRUCTION,
      MOVE_CONSTRUCTION,
      DESTRUCTION,
      COPY_ASSIGNATION,
      REF_ASSIGNATION,
      CREF_ASSIGNATION,
      MOVE_ASSIGNATION
  };

  probe() { handler(FunctionCalled::BASE_CONSTRUCTION); }
  probe(probe&) { handler(FunctionCalled::REF_CONSTRUCTION); }
  probe(const probe&) { handler(FunctionCalled::CREF_CONSTRUCTION); }
  probe(probe&&) { handler(FunctionCalled::MOVE_CONSTRUCTION); }
  ~probe() { handler(FunctionCalled::DESTRUCTION); }
  probe& operator=(probe) { handler(FunctionCalled::COPY_ASSIGNATION); return *this; }
  probe& operator=(probe&) { handler(FunctionCalled::REF_ASSIGNATION); return *this; }
  probe& operator=(const probe&) { handler(FunctionCalled::CREF_ASSIGNATION); return *this; }
  probe& operator=(probe&&) { handler(FunctionCalled::MOVE_ASSIGNATION); return *this; }

  void handler(FunctionCalled f)
  {
      static_cast<T*>(this)->specificHandler(f);
  }
};

template<>
struct probe<probe_policy::cout> : probe<probe<probe_policy::cout>> {
private:
  void specificHandler(FunctionCalled functionCalled)
  {
      switch (functionCalled) {
      case FunctionCalled::BASE_CONSTRUCTION:
          printFunctionCalled("probe()");
          break;
      case FunctionCalled::REF_CONSTRUCTION:
          printFunctionCalled("probe(&)");
          break;
      case FunctionCalled::CREF_CONSTRUCTION:
          printFunctionCalled("probe(c&)");
          break;
      case FunctionCalled::MOVE_CONSTRUCTION:
          printFunctionCalled("probe(&&)");
          break;
      case FunctionCalled::DESTRUCTION:
          printFunctionCalled("~probe()");
          break;
      case FunctionCalled::COPY_ASSIGNATION:
          printFunctionCalled("probe=()");
      case FunctionCalled::REF_ASSIGNATION:
          printFunctionCalled("probe=(&)");
          break;
      case FunctionCalled::CREF_ASSIGNATION:
          printFunctionCalled("probe=(c&)");
          break;
      case FunctionCalled::MOVE_ASSIGNATION:
          printFunctionCalled("probe=(&&)");
          break;
      }
  }

  void printFunctionCalled(const std::string& functionCallStr)
  {
      std::cout << functionCallStr << " " << (void*)(this) << std::endl;
  }
};

template <>
struct probe<probe_policy::count> : probe<probe<probe_policy::count>> {
  void specificHandler(FunctionCalled f)
  {
      switch (f) {
      case FunctionCalled::BASE_CONSTRUCTION:
          baseConstructionCount++;
          break;
      case FunctionCalled::REF_CONSTRUCTION:
          copyConstructionCount++;
          break;
      case FunctionCalled::CREF_CONSTRUCTION:
          crefConstructionCount++;
          break;
      case FunctionCalled::MOVE_CONSTRUCTION:
          moveConstructionCount++;
          break;
      default:
          // don't care about others
          break;
      }
  }

  static void resetCount()
  {
      probe<probe_policy::count>::baseConstructionCount = 0;
      probe<probe_policy::count>::copyConstructionCount = 0;
      probe<probe_policy::count>::crefConstructionCount = 0;
      probe<probe_policy::count>::moveConstructionCount = 0;
  }

  static size_t baseConstructionCount;
  static size_t copyConstructionCount;
  static size_t crefConstructionCount;
  static size_t moveConstructionCount;
};


template<class Container, class ValueType = typename Container::value_type>
void println(Container c, const std::string& delimiter = std::string(""))
{
    for (ValueType val: c) {
        std::cout << val << delimiter;
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
