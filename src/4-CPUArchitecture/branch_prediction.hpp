#pragma once
#include <playground_organizer.hpp>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <chrono>
#include <iterator>

CREATE_ELEMENT_WITH_CODE(CountIfRandom) {
  using clock = std::chrono::high_resolution_clock;
  using duration = std::chrono::duration<double, std::micro>;

  std::vector<float> v(65536);
  std::generate(std::begin(v), std::end(v), [] {
    return (rand() % 2) ? 1 : -1;
  });

  clock::time_point start = clock::now();

  int result = std::count_if(std::begin(v), std::end(v), [](float x) {
    return x > 0;
  });

  duration elapsed = clock::now() - start;
  std::cout << "duration: " << elapsed.count() / 1000.0 << " msec\n"; // return in millisecond resolution
  std::cout << "result: " << result << "\n";
}

CREATE_ELEMENT_WITH_CODE(CountIfSorted) {
  using clock = std::chrono::high_resolution_clock;
  using duration = std::chrono::duration<double, std::micro>;

  std::vector<float> v(65536);
  std::generate(std::begin(v), std::end(v), [] {
    return (rand() % 2) ? 1 : -1;
  });

  std::sort(v.begin(), v.end());

  clock::time_point start = clock::now();

  int result = std::count_if(std::begin(v), std::end(v), [](float x) {
    return x > 0;
  });

  duration elapsed = clock::now() - start;
  std::cout << "duration: " << elapsed.count() / 1000.0 << " msec\n"; // return in millisecond resolution
  std::cout << "result: " << result << "\n";
}

namespace branch_prediction
{

struct price
{
  virtual ~price() {}
  virtual float getPrice() const noexcept { return 1.0; }
};

struct cheap : public price
{
  float getPrice() const noexcept override { return 2.0; }
};

struct expensive : public price
{
  float getPrice() const noexcept override { return 3.14159; }
};

} // namespace branch_prediction

CREATE_ELEMENT_WITH_CODE(VirtualCallsSequenced) {
  using namespace branch_prediction;
  using clock = std::chrono::high_resolution_clock;
  using duration = std::chrono::duration<double, std::micro>;
  std::vector<price*> pricelist;
  std::fill_n(std::back_inserter(pricelist), 10000, new price);
  std::fill_n(std::back_inserter(pricelist), 10000, new cheap);
  std::fill_n(std::back_inserter(pricelist), 10000, new expensive);

  float sum = 0;
  clock::time_point start = clock::now();

  for (auto *p : pricelist)
  {
    sum += p->getPrice();
  }

  duration elapsed = clock::now() - start;
  std::cout << "duration: " << elapsed.count() / 1000.0 << " msec\n"; // return in millisecond resolution
  std::cout << "sum: " << sum << "\n";
}

CREATE_ELEMENT_WITH_CODE(VirtualCallsShuffled) {
  using namespace branch_prediction;
  using clock = std::chrono::high_resolution_clock;
  using duration = std::chrono::duration<double, std::micro>;
  std::vector<price*> pricelist;
  std::fill_n(std::back_inserter(pricelist), 10000, new price);
  std::fill_n(std::back_inserter(pricelist), 10000, new cheap);
  std::fill_n(std::back_inserter(pricelist), 10000, new expensive);

  std::random_shuffle(pricelist.begin(), pricelist.end());

  float sum = 0;
  clock::time_point start = clock::now();

  for (auto *p : pricelist)
  {
    sum += p->getPrice();
  }

  duration elapsed = clock::now() - start;
  std::cout << "duration: " << elapsed.count() / 1000.0 << " msec\n"; // return in millisecond resolution
  std::cout << "sum: " << sum << "\n";
}

