#pragma once
#include <playground_organizer.hpp>
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
    // aligned type with the same size of cache line
    struct alignas(64) aligned_type
    {
      std::atomic<int> val;
    };

    int test()
    {
      aligned_type a; a.val = 0; // &a: 0x...4ff240
      aligned_type b; b.val = 0; // &b: 0x...4ff280
      aligned_type c; c.val = 0; // &c: 0x...4ff2c0
      aligned_type d; d.val = 0; // &d: 0x...4ff300

      std::thread t1([&a] { work(a.val); });
      std::thread t2([&b] { work(b.val); });
      std::thread t3([&c] { work(c.val); });
      std::thread t4([&d] { work(d.val); });

      t1.join(); t2.join(); t3.join(); t4.join();
      return a.val + b.val + c.val + d.val;
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
  }

} // namespace false_sharing

CREATE_ELEMENT_WITH_CODE(FalseSharingSingleAtomic) {
  using clock = std::chrono::high_resolution_clock;
  using duration = std::chrono::duration<double, std::micro>;
  {
    std::cout << "thread count : 1\n";
    clock::time_point start = clock::now();

    auto result = false_sharing::sharing_single_atomic::test_1();

    duration elapsed = clock::now() - start;
    std::cout << "duration: " << elapsed.count() / 1000.0 << " msec\n"; // return in millisecond resolution
    std::cout << "result: " << std::to_string(result) << "\n";
  }
  {
    std::cout << "thread count : 2\n";
    clock::time_point start = clock::now();

    auto result = false_sharing::sharing_single_atomic::test_2();

    duration elapsed = clock::now() - start;
    std::cout << "duration: " << elapsed.count() / 1000.0 << " msec\n"; // return in millisecond resolution
    std::cout << "result: " << std::to_string(result) << "\n";
  }
  {
    std::cout << "thread count : 3\n";
    clock::time_point start = clock::now();

    auto result = false_sharing::sharing_single_atomic::test_3();

    duration elapsed = clock::now() - start;
    std::cout << "duration: " << elapsed.count() / 1000.0 << " msec\n"; // return in millisecond resolution
    std::cout << "result: " << std::to_string(result) << "\n";
  }
  {
    std::cout << "thread count : 4\n";
    clock::time_point start = clock::now();

    auto result = false_sharing::sharing_single_atomic::test_4();

    duration elapsed = clock::now() - start;
    std::cout << "duration: " << elapsed.count() / 1000.0 << " msec\n"; // return in millisecond resolution
    std::cout << "result: " << std::to_string(result) << "\n";
  }

}

CREATE_ELEMENT_WITH_CODE(FalseSharingAtomicsInSingleCacheLine) {
  using clock = std::chrono::high_resolution_clock;
  using duration = std::chrono::duration<double, std::micro>;
  {
    std::cout << "thread count : 2\n";
    clock::time_point start = clock::now();

    auto result = false_sharing::sharing_atomics_in_one_chache_line::test_2();

    duration elapsed = clock::now() - start;
    std::cout << "duration: " << elapsed.count() / 1000.0 << " msec\n"; // return in millisecond resolution
    std::cout << "result: " << std::to_string(result) << "\n";
  }
  {
    std::cout << "thread count : 3\n";
    clock::time_point start = clock::now();

    auto result = false_sharing::sharing_atomics_in_one_chache_line::test_3();

    duration elapsed = clock::now() - start;
    std::cout << "duration: " << elapsed.count() / 1000.0 << " msec\n"; // return in millisecond resolution
    std::cout << "result: " << std::to_string(result) << "\n";
  }
  {
    std::cout << "thread count : 4\n";
    clock::time_point start = clock::now();

    auto result = false_sharing::sharing_atomics_in_one_chache_line::test_4();

    duration elapsed = clock::now() - start;
    std::cout << "duration: " << elapsed.count() / 1000.0 << " msec\n"; // return in millisecond resolution
    std::cout << "result: " << std::to_string(result) << "\n";
  }
}
/*
  Windows içerisinde atomic deðiþkenlerin memory order yönetimleri exclusively locked (mutex ile ayný!) olduðu için
  aþaðýda 'FalseSharingResolved' içerisinde kullanýlan kodda atomik deðiþkenler farklý cache line'larda 
  yerleþmelerine raðmen mutex kullanýmý yüzünden toplam iþlem süresi diðerlerinden farklý olmayacaktýr!
  Linux veya unix (mac) iþletim sistemlerinde atomik yapýlarýn memory order yönetimi esnek çalýþtýðýndan sonuç,
  sunumdaki grafik gibi olacaktýr.
*/
CREATE_ELEMENT_WITH_CODE(FalseSharingResolved) {
  using clock = std::chrono::high_resolution_clock;
  using duration = std::chrono::duration<double, std::micro>;
  {
    std::cout << "thread count : 4\n";
    clock::time_point start = clock::now();

    auto result = false_sharing::false_sharing_resolved::test();

    duration elapsed = clock::now() - start;
    std::cout << "duration: " << elapsed.count() / 1000.0 << " msec\n"; // return in millisecond resolution
    std::cout << "result: " << std::to_string(result) << "\n";
  }
  {
    std::cout << "thread local variable: \n";
    clock::time_point start = clock::now();

    auto result = false_sharing::false_sharing_resolved_noatomic::test();

    duration elapsed = clock::now() - start;
    std::cout << "duration: " << elapsed.count() / 1000.0 << " msec\n"; // return in millisecond resolution
    std::cout << "result: " << std::to_string(result) << "\n";
  }
}
