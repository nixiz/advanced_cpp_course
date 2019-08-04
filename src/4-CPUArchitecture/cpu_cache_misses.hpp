#pragma once
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


  template <class T>
  class Matrix {
  public:
    typedef typename std::decay<T>::type Type;

    explicit Matrix(long long dim_size) :
      rowsize(dim_size / 2), columnsize(dim_size - rowsize)
    {
      // check if matrix is equal sized
      assert(rowsize == columnsize);
      
      std::default_random_engine generator;
      std::uniform_int_distribution<Type> distribution(1, dim_size);

      data_ = new Type*[rowsize];
      for (size_t i = 0; i < rowsize; i++)
      {
        data_[i] = new Type[columnsize];
        // set initial values
        for (size_t j = 0; j < columnsize; j++)
        {
          // set random initial data
          data_[i][j] = distribution(generator);
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

    unsigned int rows() const {
      return rowsize;
    }

    unsigned int columns() const {
      return columnsize;
    }
    
    Type* operator[](unsigned int index) {
      return data_[index];
    }

    Type* operator[](unsigned int index) const {
      return data_[index];
    }

    /*
    // checked way access of array
    struct two_dim_arr_access_helper {
      explicit two_dim_arr_access_helper(T* _a) noexcept : arr(_a) { }

      T& operator[](unsigned int index) {
        assert(arr[index] != nullptr);
        return arr[index];
      }

      T const& operator[](unsigned int index) const {
        assert(arr[index] != nullptr);
        return arr[index];
      }
    private:
      T* arr;
    };

    two_dim_arr_access_helper operator[](unsigned int index) {
      assert(data_[index] != nullptr);
      return two_dim_arr_access_helper(data_[index]);
    }

    two_dim_arr_access_helper operator[](unsigned int index) const {
      assert(data_[index] != nullptr);
      return two_dim_arr_access_helper(data_[index]);
    }
    */
  private:
    Type** data_;
    unsigned int rowsize;
    unsigned int columnsize;
  };
  
  template <typename T>
  void sumMatrix(const Matrix<T>& m,
                 long long& sum, TraversalOrder order)
  {
    sum = 0;
    if (order == TraversalOrder::RowMajor) {
      for (unsigned r = 0; r < m.rows(); ++r) {
        for (unsigned c = 0; c < m.columns(); ++c) {
          sum += m[r][c];
        }
      }
    }
    else {
      for (unsigned c = 0; c < m.columns(); ++c) {
        for (unsigned r = 0; r < m.rows(); ++r) {
          sum += m[r][c];
        }
      }
    }
  }

  class MatrixSumBenchmarkHelper {
  public:
    using clock = std::chrono::high_resolution_clock;
    using duration = std::chrono::duration<double, std::micro>;

    explicit MatrixSumBenchmarkHelper(long long matrix_size) : 
      mat(matrix_size) { }

    double start(TraversalOrder order) {
      long long sum = 0;
      clock::time_point start = clock::now();
      
      sumMatrix(mat, sum, order);

      duration elapsed = clock::now() - start;
      return elapsed.count() / 1000.0; // return in millisecond resolution
    }

  protected:
    Matrix<int> mat;
  };

  struct benchmarkresult {
    void SetName(std::string name) {
      testname = name;
    }

    void AddSample(long long matrix_size, double time) {
      samplings.push_back(
        std::make_pair(matrix_size, time));
    }

    friend std::ostream& operator<<(std::ostream& os, const benchmarkresult& result);
  private:
    std::string testname;
    std::vector<std::pair<long long, double>> samplings; // matrix size (mb) - traversal sum time in milliseconds
  };
  
  std::ostream& operator<<(std::ostream& os, const benchmarkresult& result) {
    os
      << "results of: " << result.testname << "\n"
      << "matrix size (MB)\telapsed time (ms)" << std::setw(5) << "\n";
    for (size_t i = 0; i < result.samplings.size(); i++)
    {
      os << "\t" << result.samplings[i].first << "\t\t" << result.samplings[i].second << "\n";
    }
    return os;
  }

} // namespace cpu_cache_misses

CREATE_ELEMENT_WITH_CODE(CpuCacheExample) {
  using namespace cpu_cache_misses;
  constexpr size_t num_of_iter = 5;
  constexpr size_t done_before = 0;

  // initiate result map
  std::map<std::string, benchmarkresult> results;
  results["rowmajor"].SetName("rowmajor");
  results["columnmajor"].SetName("columnmajor");

  for (size_t i = 0; i < num_of_iter; i++)
  {
    auto dimsize = 1024 << (i + done_before);
    std::cout
      << "\n------------------benchmark point------------------\n"
      << "Matrix with size : " << dimsize << std::endl;
    MatrixSumBenchmarkHelper benchmarker{ dimsize };
    std::cout
      << "Starting benchmark for row major traversal" << std::endl;
    results["rowmajor"].AddSample(dimsize, benchmarker.start(TraversalOrder::RowMajor));

    std::cout
      << "Starting benchmark for column major traversal" << std::endl;
    results["columnmajor"].AddSample(dimsize, benchmarker.start(TraversalOrder::ColumnMajor));
  }

  std::cout
    << "\n------------------------------------------------------\n"
    << results["rowmajor"]
    << "\n------------------------------------------------------\n"
    << results["columnmajor"]
    << std::endl;
}
