#pragma once
#include <chrono>
#include <type_traits>
#include <utility>
#include <string>
#include <variant>
#include <vector>
#include <algorithm>
#include <numeric>

template <class T>
struct return_type_holder
{
  using type = T;
};

template <>
struct return_type_holder<void>
{
  using type = std::monostate;
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
  constexpr static inline size_t maximum_sample_size = 100;

  template <typename Fun, typename ...Args>
  static inline auto run(Fun&& func, Args&& ...args) -> std::pair<duration_type, typename return_type_holder<decltype(std::forward<Fun>(func)(std::forward<Args>(args)...))>::type>
  {
    using func_return_type = std::invoke_result_t<Fun, Args...>;
    double total_duration{}, mean{}, total_stdev{};
    std::vector<double> elapsed_times;
    std::vector<double> stdev_list;
    size_t sample_size = 0;
    duration elapsed{};
    elapsed_times.reserve(benchmarker::maximum_sample_size);
    stdev_list.reserve(benchmarker::maximum_sample_size);
    auto calc_stdev = [] (const double mean, const std::vector<double>& samples) -> double
    {
      const auto num_samples = samples.size();
      std::vector<double> variance_mean(num_samples);
      std::transform(begin(samples), end(samples), begin(variance_mean), [mean](double e) { return e - mean; });
      const double sq_sum = std::inner_product(begin(variance_mean), end(variance_mean), begin(variance_mean), 0.0);
      const double stdev = std::sqrt(sq_sum / num_samples);
      return stdev;
    };

    using ret_type = std::variant<typename return_type_holder<func_return_type>::type, std::monostate>;
    ret_type invoke_result_variant;
    do
    {
      const auto start = clock::now();
      invoke_result_variant = benchmarker::invoke_func(std::forward<Fun>(func), std::forward<Args>(args)...);
      elapsed = clock::now() - start;
      const auto elapsed_time = elapsed.count();
      elapsed_times.push_back(elapsed_time);
      total_duration += elapsed_time;
      mean = total_duration / elapsed_times.size();

      auto stdev_1 = calc_stdev(mean, elapsed_times);
      total_stdev += stdev_1;
      stdev_list.push_back(stdev_1);

      const auto stdev_mean = total_stdev / stdev_list.size();
      const auto stdev = calc_stdev(stdev_mean, stdev_list);

      if (stdev > 0 && (stdev / mean) < 0.03) {
        break; // warm-up state is done
      }
    } while ((++sample_size <= benchmarker::maximum_sample_size) &&
             (elapsed <= std::chrono::milliseconds(823)));

    auto [mmin, mmax] = std::minmax_element(std::begin(elapsed_times), std::end(elapsed_times));
    std::cout << std::fixed
      << "benchmark results >>> "
      << "total samples: " << elapsed_times.size()
      << "| mean: " << mean
      << "| min: " << *mmin
      << "| max: " << *mmax << " us\n";

    return std::make_pair(mean / 1000.0, std::get<func_return_type>(invoke_result_variant));
  }

private:

  template <typename Fun, typename ...Args>
  [[nodiscard]] static auto invoke_func(Fun&& func, Args&& ...args) -> std::variant<typename return_type_holder<std::invoke_result_t<Fun, Args...>>::type, std::monostate>
  {
    using func_return_type = std::invoke_result_t<Fun, Args...>;
    using ret_type = std::variant<typename return_type_holder<std::invoke_result_t<Fun, Args...>>::type, std::monostate>;
    if constexpr (std::is_void_v<func_return_type>)
    {
      std::invoke(std::forward<Fun>(func), std::forward<Args>(args)...);
      return ret_type{std::monostate{}};
    }
    else
    {
      func_return_type res = std::invoke(std::forward<Fun>(func), std::forward<Args>(args)...);
      return ret_type{res};
    }
  }


};

#ifdef _WIN32
// to_string overload for void types 
_STD_BEGIN
_NODISCARD inline string to_string(std::monostate)
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