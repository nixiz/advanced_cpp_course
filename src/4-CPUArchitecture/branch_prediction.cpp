#include <advanced_cpp_topics.h>
#include <benchmarker.hpp>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <chrono>
#include <iterator>

ELEMENT_CODE(CountIfRandom) {
  std::vector<int> v(65536);
  std::generate(std::begin(v), std::end(v), [] {
    return (rand() % 2) ? 1 : -1;
  });

  auto result = benchmarker::run([&] {
    auto count = std::count_if(std::begin(v), std::end(v), [](auto x) {
      return x > 0;
    });
    return count;
  });

  std::cout << "duration: " << result.first << " msec\n";
  std::cout << "result: " << result.second << "\n";
}

ELEMENT_CODE(CountIfSequenced) {
  std::vector<int> v(65536);
  std::generate(std::begin(v), std::end(v), [n = 0]() mutable {
    return (++n % 2) ? 1 : -1;
  });

  auto result = benchmarker::run([&] {
    auto count = std::count_if(std::begin(v), std::end(v), [](auto x) {
      return x > 0;
    });
    return count;
  });

  std::cout << "duration: " << result.first << " msec\n";
  std::cout << "result: " << result.second << "\n";
}

ELEMENT_CODE(CountIfSorted) {
  std::vector<int> v(65536);
  std::generate(std::begin(v), std::end(v), [] {
    return (rand() % 2) ? 1 : -1;
  });

  std::sort(v.begin(), v.end());

  auto result = benchmarker::run([&] {
    auto count = std::count_if(std::begin(v), std::end(v), [](auto x) {
      return x > 0;
    });
    return count;
  });

  std::cout << "duration: " << result.first << " msec\n";
  std::cout << "result: " << result.second << "\n";
}

namespace branch_prediction
{
  struct price
  {
    virtual ~price() {}
    virtual float getPrice() const noexcept { return 1.0f; }
  };

  struct cheap : public price
  {
    float getPrice() const noexcept override { return 2.0f; }
  };

  struct expensive : public price
  {
    float getPrice() const noexcept override { return 3.14159f; }
  };

} // namespace branch_prediction

ELEMENT_CODE(VirtualCallsSequenced) {
  using namespace branch_prediction;
  std::vector<price*> pricelist;
  std::fill_n(std::back_inserter(pricelist), 10000, new price);
  std::fill_n(std::back_inserter(pricelist), 10000, new cheap);
  std::fill_n(std::back_inserter(pricelist), 10000, new expensive);

  auto result = benchmarker::run([&] {
    float sum = 0;
    for (auto *p : pricelist)
    {
      sum += p->getPrice();
    }
    return sum;
  });

  std::cout << "duration: " << result.first << " msec\n"; // return in millisecond resolution
  std::cout << "sum: " << result.second << "\n";
}

ELEMENT_CODE(VirtualCallsShuffled) {
  using namespace branch_prediction;
  std::random_device rd;
  std::mt19937 g{ rd() };
  std::vector<price*> pricelist;
  std::fill_n(std::back_inserter(pricelist), 10000, new price);
  std::fill_n(std::back_inserter(pricelist), 10000, new cheap);
  std::fill_n(std::back_inserter(pricelist), 10000, new expensive);

  std::shuffle(pricelist.begin(), pricelist.end(), g);

  auto result = benchmarker::run([&] {
    float sum = 0;
    for (auto *p : pricelist)
    {
      sum += p->getPrice();
    }
    return sum;
  });

  std::cout << "duration: " << result.first << " msec\n"; // return in millisecond resolution
  std::cout << "sum: " << result.second << "\n";
}

