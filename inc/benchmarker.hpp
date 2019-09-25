#pragma once
#include <chrono>
#include <type_traits>
#include <utility>
#include <string>


template <class T>
struct return_type_holder
{
  using type = T;
};

template <>
struct return_type_holder<void>
{
  struct empty_struct {};
  using type = empty_struct;
};

/*
  TODO(oguzhank): bu sekilde bir benchmarker güzel olabilir
  benchmarker()
    .prepare([]{
      std::cout << "thread count : 2\n";
      return 5;
    })
    .run([](const auto& val) {
      static_assert(val == 2, "");
      int sum = 0;
      for(int i=0; i<val; ++i)
      {
        sum += 1;
      }
      ret sum; // sum => 1 + 2 + 3 + 4 + 5
    })
    .then([](const auto& duration, const auto& result){
      static_assert(result == (1 + 2 + 3 + 4 + 5), "");
      std::cout << "duration: " << duration << " msec\n";
      std::cout << "result: " << std::to_string(result) << "\n";
    });
*/
struct benchmarker
{
  using clock = std::chrono::high_resolution_clock;
  using duration = std::chrono::duration<double, std::micro>;
  using duration_type = duration::rep;

  template <typename Fun, typename ...Args>
  static inline auto run(Fun&& func, Args&& ...args) -> std::pair<duration_type, typename return_type_holder<decltype(std::forward<Fun>(func)(std::forward<Args>(args)...))>::type>
  {
    using return_type = decltype(std::forward<Fun>(func)(std::forward<Args>(args)...));
    duration elapsed;
    clock::time_point start = clock::now();

    if constexpr (std::is_void_v<return_type>)
    {
      std::forward<Fun>(func)(std::forward<Args>(args)...);
      elapsed = clock::now() - start;
      return std::make_pair(elapsed.count() / 1000.0, return_type_holder<void>::empty_struct{});
    }
    else
    {
      return_type res = std::forward<Fun>(func)(std::forward<Args>(args)...);
      elapsed = clock::now() - start;
      return std::make_pair(elapsed.count() / 1000.0, std::move(res));
    }
  }

};

#ifdef _WIN32
// to_string overload for void types 
_STD_BEGIN
_NODISCARD inline string to_string(return_type_holder<void>::empty_struct)
{	// convert void to string
  return string{};
}
_STD_END
#else
namespace std {
  [[nodiscard]] inline string to_string(return_type_holder<void>::empty_struct)
  {	// convert void to string
    return string{};
  }
}
#endif