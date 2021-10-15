#include <advanced_cpp_topics.h>
#include <benchmarker.hpp>
#include <atomic>
#include <thread>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <chrono>

namespace false_sharing
{
  void work(std::atomic<int>& a)
  {
    for (int i = 0; i < 10000; ++i)
      a++;
  }

  void work_with_sentinel(std::atomic<int>& a)
  {
    unsigned int sentinel = 0;
    for (int i = 0; i < 10000; ++i)
      ++sentinel;
    a += sentinel;
  }

  namespace sharing_single_atomic
  {
    int test_1()
    {
      std::atomic<int> a; a = 0;

      std::thread t1([&] { work(a); });

      t1.join();
      return a;
    }

    int test_2()
    {
      std::atomic<int> a; a = 0;

      std::thread t1([&] { work(a); });
      std::thread t2([&] { work(a); });

      t1.join(); t2.join();
      return a;
    }

    int test_3()
    {
      std::atomic<int> a; a = 0;

      std::thread t1([&] { work(a); });
      std::thread t2([&] { work(a); });
      std::thread t3([&] { work(a); });

      t1.join(); t2.join(); t3.join();
      return a;
    }

    int test_4()
    {
      std::atomic<int> a; a = 0;

      std::thread t1([&] { work(a); });
      std::thread t2([&] { work(a); });
      std::thread t3([&] { work(a); });
      std::thread t4([&] { work(a); });

      t1.join(); t2.join(); t3.join(); t4.join();
      return a;
    }

    int test_4_sentinel()
    {
      std::atomic<int> a; a = 0;

      std::thread t1([&] { work_with_sentinel(a); });
      std::thread t2([&] { work_with_sentinel(a); });
      std::thread t3([&] { work_with_sentinel(a); });
      std::thread t4([&] { work_with_sentinel(a); });

      t1.join(); t2.join(); t3.join(); t4.join();
      return a;
    }
  }

  namespace sharing_atomics_in_one_chache_line
  {
    int test_2()
    {
      std::atomic<int> a; a = 0;
      std::atomic<int> b; b = 0;

      std::thread t1([&] { work(a); });
      std::thread t2([&] { work(b); });

      t1.join(); t2.join();
      return a + b;
    }

    int test_3()
    {
      std::atomic<int> a; a = 0;
      std::atomic<int> b; b = 0;
      std::atomic<int> c; c = 0;

      std::thread t1([&] { work(a); });
      std::thread t2([&] { work(b); });
      std::thread t3([&] { work(c); });

      t1.join(); t2.join(); t3.join();
      return a + b + c;
    }

    int test_4()
    {
      std::atomic<int> a; a = 0; // &a: 0x...b2f7c0
      std::atomic<int> b; b = 0; // &b: 0x...b2f7c4
      std::atomic<int> c; c = 0; // &c: 0x...b2f7c8
      std::atomic<int> d; d = 0; // &d: 0x...b2f7cc

      std::thread t1([&] { work(a); });
      std::thread t2([&] { work(b); });
      std::thread t3([&] { work(c); });
      std::thread t4([&] { work(d); });

      t1.join(); t2.join(); t3.join(); t4.join();
      return a + b + c + d;
    }
  }

  namespace false_sharing_resolved
  {
    int test()
    {
      // aligned type with the same size of cache line
      alignas(64) std::atomic<int> a{}; // &a: 0x...4ff240
      alignas(64) std::atomic<int> b{}; // &b: 0x...4ff280
      alignas(64) std::atomic<int> c{}; // &c: 0x...4ff2c0
      alignas(64) std::atomic<int> d{}; // &d: 0x...4ff300

      std::thread t1([&a] { work(a); });
      std::thread t2([&b] { work(b); });
      std::thread t3([&c] { work(c); });
      std::thread t4([&d] { work(d); });

      t1.join(); t2.join(); t3.join(); t4.join();
      return a + b + c + d;
    }
  }

  namespace false_sharing_resolved_noatomic
  {
    struct alignas(64) aligned_type
    {
      unsigned int val;
    };

    void work(unsigned int& val)
    {
      for (int i = 0; i < 10000; ++i)
        val++;
    }

    void work_with_sentinel(unsigned int& val)
    {
      unsigned int sentinel = 0;
      for (int i = 0; i < 10000; ++i)
        ++sentinel;
      val = sentinel;
    }

    int test()
    {
      aligned_type a; a.val = 0; // &a: 0x...70f500
      aligned_type b; b.val = 0; // &b: 0x...70f4c0
      aligned_type c; c.val = 0; // &c: 0x...70f480
      aligned_type d; d.val = 0; // &d: 0x...70f440

      std::thread t1([&] { work(a.val); });
      std::thread t2([&] { work(b.val); });
      std::thread t3([&] { work(c.val); });
      std::thread t4([&] { work(d.val); });

      t1.join(); t2.join(); t3.join(); t4.join();
      return a.val + b.val + c.val + d.val;
    }

    int test_sentinel()
    {
      aligned_type a; a.val = 0; // &a: 0x...70f500
      aligned_type b; b.val = 0; // &b: 0x...70f4c0
      aligned_type c; c.val = 0; // &c: 0x...70f480
      aligned_type d; d.val = 0; // &d: 0x...70f440

      std::thread t1([&] { work_with_sentinel(a.val); });
      std::thread t2([&] { work_with_sentinel(b.val); });
      std::thread t3([&] { work_with_sentinel(c.val); });
      std::thread t4([&] { work_with_sentinel(d.val); });

      t1.join(); t2.join(); t3.join(); t4.join();
      return a.val + b.val + c.val + d.val;
    }
  }

} // namespace false_sharing

ELEMENT_CODE(FalseSharingSingleAtomic) {
  using namespace false_sharing::sharing_single_atomic;
  auto print_result = [](const auto& result) {
    std::cout << "duration: " << result.first << " msec\n";
    std::cout << "result: " << std::to_string(result.second) << "\n";
  };

  std::cout << "thread count : 1\n";
  auto result_1 = benchmarker::run(test_1);
  print_result(result_1);

  std::cout << "thread count : 2\n";
  auto result_2 = benchmarker::run(test_2);
  print_result(result_2);

  std::cout << "thread count : 3\n";
  auto result_3 = benchmarker::run(test_3);
  print_result(result_3);

  std::cout << "thread count : 4\n";
  auto result_4 = benchmarker::run(test_4);
  print_result(result_4);

  std::cout << "[with sentinel] thread count : 4\n";
  auto result_5 = benchmarker::run(test_4_sentinel);
  print_result(result_5);
}

ELEMENT_CODE(FalseSharingAtomicsInSingleCacheLine) {
  using namespace false_sharing::sharing_atomics_in_one_chache_line;
  auto print_result = [](const auto& result) {
    std::cout << "duration: " << result.first << " msec\n";
    std::cout << "result: " << std::to_string(result.second) << "\n";
  };
  std::cout << "thread count : 2\n";
  print_result(benchmarker::run(test_2));

  std::cout << "thread count : 3\n";
  print_result(benchmarker::run(test_3));

  std::cout << "thread count : 4\n";
  print_result(benchmarker::run(test_4));
}
/*
  Windows i�erisinde atomic de�i�kenlerin memory order y�netimleri exclusively locked (mutex ile ayn�!) oldu�u i�in
  a�a��da 'FalseSharingResolved' i�erisinde kullan�lan kodda atomik de�i�kenler farkl� cache line'larda
  yerle�melerine ra�men mutex kullan�m� y�z�nden toplam i�lem s�resi di�erlerinden farkl� olmayacakt�r!
  Linux veya unix (mac) i�letim sistemlerinde atomik yap�lar�n memory order y�netimi esnek �al��t���ndan sonu�,
  sunumdaki grafik gibi olacakt�r.
*/
ELEMENT_CODE(FalseSharingResolved) {
  auto print_result = [](const auto& result) {
    std::cout << "duration: " << result.first << " msec\n";
    std::cout << "result: " << std::to_string(result.second) << "\n";
  };
  std::cout << "thread count : 4\n";
  print_result(benchmarker::run(false_sharing::false_sharing_resolved::test));

  std::cout << "thread local variable: \n";
  print_result(benchmarker::run(false_sharing::false_sharing_resolved_noatomic::test));

  std::cout << "thread local variable with sentinel: \n";
  print_result(benchmarker::run(false_sharing::false_sharing_resolved_noatomic::test_sentinel));
}
