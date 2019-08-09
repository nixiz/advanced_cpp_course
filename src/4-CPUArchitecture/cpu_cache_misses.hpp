#pragma once
#include <limits>
#include <playground_organizer.hpp>
#include <type_traits>
#include <chrono>
#include <random>
#include <map>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

namespace cpu_cache_misses {
  
  enum class TraversalOrder : unsigned {
    RowMajor,
    ColumnMajor
  };

  template <unsigned int size>
  struct random_map
  {
    unsigned int map[size] = { 0 };
    random_map()
    {
      srand(time(NULL));
      std::generate(std::begin(map), std::end(map), [] {
        return rand();
      });
    }

    unsigned int operator()() const
    {
      return map[++indx % size];
    }
  private:
    mutable unsigned int indx = 0;
  };
  const static random_map<1024> random_numbers;

  template <class T>
  class Matrix {
  public:
    typedef typename std::decay<T>::type Type;

    explicit Matrix(unsigned long long matrix_size)
    {
      const unsigned long long matrix_dim = static_cast<unsigned long long>(sqrt(matrix_size));
      const auto dim_size = matrix_dim % 2 == 0 ? matrix_dim : matrix_dim + 1;
      rowsize    = (dim_size / 2);
      columnsize = (dim_size - rowsize);
      // check if matrix is equal sized
      assert(rowsize == columnsize);
      
      //std::default_random_engine generator;
      //std::uniform_int_distribution<int> distribution(1, (std::numeric_limits<Type>::max)());

      data_ = new Type*[rowsize];
      for (unsigned long long i = 0; i < rowsize; i++)
      {
        data_[i] = new Type[columnsize];
        // set initial values
        for (unsigned long long j = 0; j < columnsize; j++)
        {
          // set random initial data
          data_[i][j] = static_cast<Type>(random_numbers());
        }
      }
    }

    ~Matrix()
    {
      // delete allocated source
      for (size_t i = 0; i < rowsize; i++)
      {
        delete[] data_[i];
      }
      delete[] data_;
    }

    unsigned long long rows() const {
      return rowsize;
    }

    unsigned long long columns() const {
      return columnsize;
    }
    
    Type* operator[](unsigned long long index) {
      return data_[index];
    }

    Type* operator[](unsigned long long index) const {
      return data_[index];
    }
  private:
    Type** data_;
    unsigned long long rowsize;
    unsigned long long columnsize;
  };
  
  template <typename T>
  unsigned long long sumMatrix(const Matrix<T>& m, TraversalOrder order)
  {
    unsigned long long sum = 0;
    if (order == TraversalOrder::RowMajor) {
      for (unsigned long long r = 0; r < m.rows(); ++r) {
        for (unsigned long long c = 0; c < m.columns(); ++c) {
          sum += m[r][c];
        }
      }
    }
    else {
      for (unsigned long long c = 0; c < m.columns(); ++c) {
        for (unsigned long long r = 0; r < m.rows(); ++r) {
          sum += m[r][c];
        }
      }
    }
    return sum;
  }

  class MatrixSumBenchmarkHelper {
  public:
    using clock = std::chrono::high_resolution_clock;
    using duration = std::chrono::duration<double, std::micro>;

    explicit MatrixSumBenchmarkHelper(unsigned long long matrix_size) : 
      mat(matrix_size) { }

    std::pair<double, unsigned long long> start(TraversalOrder order) {
      unsigned long long sum = 0;
      clock::time_point start = clock::now();
      
      sum = sumMatrix(mat, order);

      duration elapsed = clock::now() - start;
      // return in millisecond resolution
      return std::make_pair(elapsed.count() / 1000.0, sum);
    }

  protected:
    Matrix<unsigned char> mat;
  };

  struct benchmarkresult {
    void SetName(std::string name) {
      testname = name;
    }

    void AddSample(unsigned long long matrix_size, double time) {
      samplings.push_back(
        std::make_pair(matrix_size, time));
    }

    friend std::ostream& operator<<(std::ostream& os, const benchmarkresult& result);
  private:
    std::string testname;
    std::vector<std::pair<unsigned long long, double>> samplings; // matrix size (mb) - traversal sum time in milliseconds
  };
  
  std::ostream& operator<<(std::ostream& os, const benchmarkresult& result) {
    os
      << "results of: " << result.testname << "\n"
      << "matrix size (MB)\telapsed time (ms)" << std::setw(5) << "\n";
    for (size_t i = 0; i < result.samplings.size(); i++)
    {
      os << "\t" << result.samplings[i].first / (1024*1024) << "\t\t" << result.samplings[i].second << "\n";
    }
    return os;
  }

} // namespace cpu_cache_misses

CREATE_ELEMENT_WITH_CODE(CpuCacheExample) {
  using namespace cpu_cache_misses;
  constexpr size_t num_of_iter = 100;

  // initiate result map
  std::map<std::string, benchmarkresult> results;
  results["rowmajor"].SetName("rowmajor");
  results["columnmajor"].SetName("columnmajor");

  for (size_t i = 1; i <= num_of_iter; ++i)
  {
    unsigned long long matrix_size = (1024 * 1024) * (i * 5);
    std::cout
      << "\n------------------benchmark point------------------\n"
      << "Matrix with size : " << matrix_size << std::endl;
    MatrixSumBenchmarkHelper benchmarker{ matrix_size };
    std::cout
      << "Starting benchmark for row major traversal" << std::endl;
    auto benchmark_result = benchmarker.start(TraversalOrder::RowMajor);
    results["rowmajor"].AddSample(matrix_size, benchmark_result.first);
    std::cout << "sum: " << benchmark_result.second << "\n";

    std::cout
      << "Starting benchmark for column major traversal" << std::endl;
    auto benchmark_result_column = benchmarker.start(TraversalOrder::ColumnMajor);
    results["columnmajor"].AddSample(matrix_size, benchmark_result_column.first);
    std::cout << "sum: " << benchmark_result_column.second << "\n";
  }

  std::cout
    << "\n------------------------------------------------------\n"
    << results["rowmajor"]
    << "\n------------------------------------------------------\n"
    << results["columnmajor"]
    << std::endl;
}
