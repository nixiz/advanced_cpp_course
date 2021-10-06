#include <advanced_cpp_topics.h>
#include <iostream>
#include <string>
#include <vector>

#include <algorithm>
#include <numeric>
#include <future>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

namespace asyncusage {

  template<typename T, int N>
  struct CompileTimeArray {
    constexpr CompileTimeArray() : arr(), len(N - 1), endIndex(N - 1) {
      while (len > 0)
      {
        arr[len] = len;
        len = len - 1;
      }
    }

    constexpr T operator[] (std::size_t index) const {
      return arr[index];
    }

    constexpr std::size_t size() const {
      return len;
    }

    T arr[N];
    std::size_t endIndex;
  private:
    int len;
  };

  template <typename RandomIt, size_t AsyncThreshold = 1000>
  int parallel_sum(RandomIt beg, RandomIt end)
  {
    std::cout << "running thread id: " << std::this_thread::get_id() << std::endl;
    auto len = end - beg;
    if (len < AsyncThreshold) {
      return std::accumulate(beg, end, 0, [](auto x, auto y) {
        //return after some delay.
        std::this_thread::sleep_for(1ms); // sleep 1 millisecond for every calculation
        return x + y;
      });
    }

    RandomIt mid = beg + len / 2;
    auto handle = std::async(std::launch::async,
      parallel_sum<RandomIt>, mid, end);
    int sum = parallel_sum(beg, mid);
    return sum + handle.get();
  }

} // namespace asyncusage 

ELEMENT_CODE(AsyncUsageExample) {
  using namespace asyncusage;
  // list generated at compile time.
  constexpr auto list = CompileTimeArray<int, 10000>();
  auto sum = parallel_sum(&list.arr[0], &list.arr[list.endIndex]);
  std::cout << "Sum of array: " << sum << std::endl;
}

ELEMENT_CODE(AsyncSequentialExample) {
  using namespace asyncusage;
  // list generated at compile time.
  constexpr auto list = CompileTimeArray<int, 10000>();
  auto sum = parallel_sum<decltype(&list.arr[0]), 10001>(&list.arr[0], &list.arr[list.endIndex]);
  std::cout << "Sum of array: " << sum << std::endl;
}

namespace asyncfutureusage {

  int BigSlowFunction() {
    std::this_thread::sleep_for(1s);
    return 5;
  }

  void BigSlowFunctionPromise(std::promise<int>& _promise) {
    _promise.set_value_at_thread_exit(5);
    std::this_thread::sleep_for(1s);
  }

  void FutureUsage() {
    std::future<int> result = std::async(std::launch::async, BigSlowFunction);
  } // result will wait operation done on destructor

  std::future<int> FutureUsageReturnFuture() {
    std::future<int> result = std::async(std::launch::async, BigSlowFunction);
    return result;
  } // 

  /*
  std::future<int> FutureUsageReturnFutureMove() {
    std::future<int> result = std::async(BigSlowFunction);
    return std::move(result);
  } //
  */

  int FutureUsagePromise() {
    std::promise<int> prom;
    std::future<int> result = prom.get_future();

    // call in another thread
    std::thread t{ BigSlowFunctionPromise, std::ref(prom) };
    t.detach();
    // result will wait the operation done
    return result.get();
  } // 

} // namespace asyncfutureusage 

ELEMENT_CODE(AsyncFutureUsageExample) {
  using namespace asyncfutureusage;
  {
    std::cout << "Calling FutureUsage()" << std::endl;
    FutureUsage();
    std::cout << "this will be printed after 1 second" << std::endl;
  }

  {
    std::cout << "Calling FutureUsageReturnFuture()" << std::endl;
    auto ret = FutureUsageReturnFuture(); // or FutureUsageReturnFutureMove() would be same!
    std::cout << "this will be printed immediately after the calling FutureUsageReturnFuture()" << std::endl;
    std::cout << "this result print waits the opeation done. result is : " << ret.get() << std::endl;
  }

  {
    std::cout << "Calling FutureUsagePromise()" << std::endl;
    auto ret = FutureUsagePromise();
    std::cout << "this will be printed after 1 second" << std::endl;
  }




}
