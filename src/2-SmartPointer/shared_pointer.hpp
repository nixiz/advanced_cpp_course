#pragma once
#include <playground_organizer.hpp>
#include <vector>
#include <thread>
#include <chrono>
#include <future>
#include "inc/number.hpp"

namespace sharedpointer {


  auto shared_ptr_usage()
  {
    std::shared_ptr<Number> sharedNumber{ new Number{10} };
    std::cout << "\nprocess[thread: " << std::this_thread::get_id() << "] started\n";
    std::cout << "use count for shared_ptr<Number>: " << sharedNumber.use_count() << std::endl;

    std::thread second_thread([](std::shared_ptr<Number> num) {
      std::cout << "process[thread: " << std::this_thread::get_id() << "] started\n";
      std::cout << "use count for shared_ptr<Number>: " << num.use_count() << std::endl;
      unsigned count = 0;
      while (count < 3) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        if (rand() % 10 == 0) ++count;
      }
      std::cout << "process[thread: " << std::this_thread::get_id() << "] done and will be destroyed\n";
    }, sharedNumber);

    unsigned count = 0;
    while (count < 3) {
      std::this_thread::sleep_for(std::chrono::milliseconds(200));
      if (rand() % 10 == 0) ++count;
    }
    std::cout << "process[thread: " << std::this_thread::get_id() << "] done and will be destroyed\n";
    return std::move(second_thread);
  }

  struct SharedNumber : Number, std::enable_shared_from_this<SharedNumber> {
    std::shared_ptr<SharedNumber> getptr() {
      return shared_from_this();
    }
    SharedNumber(int initial) : Number(initial) { }

    int increment()
    {
      int current_val = this->get();
      this->set(++current_val);
      return current_val;
    }

    int decrement()
    {
      int current_val = this->get();
      this->set(--current_val);
      return current_val;
    }
  };

  auto usage_enable_shared_from_this()
  {
    //auto = std::shared_ptr<SharedNumber>
    auto shared_number_ptr = std::make_shared<SharedNumber>(10);

    auto func = [](SharedNumber *num) {
      return std::thread(
        [](auto num_ptr) { // thread function
        while (num_ptr->decrement() > 0) {
          std::this_thread::sleep_for(std::chrono::milliseconds(200));
          std::cout << num_ptr->toString() << "\n";
        }
      },
        num->shared_from_this() // function arguments
        );
    };
    // send SharedNumber as raw pointer and exit from the scope
    return std::move(func(shared_number_ptr.get()));
  }

  void usage_weak_ptr()
  {
    auto shared_number_ptr = std::make_shared<SharedNumber>(10);
    std::weak_ptr<SharedNumber> weak_ptr = shared_number_ptr;
    std::thread([](std::weak_ptr<SharedNumber> num_ptr) {
      while (!num_ptr.expired())
      {
        auto sp = num_ptr.lock();
        if (sp->decrement() <= 0) break;
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << sp->toString() << "\n";
      }
    }, std::move(weak_ptr)).detach();

    std::this_thread::sleep_for(
      std::chrono::milliseconds((rand() % 300) + 200));
  }

  class SharedPointerReleasePool
  {
  public:
    SharedPointerReleasePool() : is_active(true)
    {
      timer = std::thread([pool = this] {
        while (pool->is_active)
        {
          std::this_thread::sleep_for(std::chrono::milliseconds(1000));
          pool->CheckAndClearPointers();
        }
      });
    }

    template <typename T>
    void Append(const std::shared_ptr<T>& object)
    {
      if (!object) return;
      std::lock_guard<std::mutex> lock(guard);
      pool.push_back(object);
    }

    ~SharedPointerReleasePool()
    {
      is_active = false;
      timer.join();
      pool.clear();
    }

    void CheckAndClearPointers() noexcept
    {
      std::lock_guard<std::mutex> lock(guard);
      pool.erase(
        std::remove_if(std::begin(pool), std::end(pool), [](const auto& ptr) {
        return ptr.use_count() <= 1;
      }), pool.end());
    }
    std::atomic<bool> is_active;
  private:
    std::vector<std::shared_ptr<void>> pool;
    std::mutex guard;
    std::thread timer;
  };

  void usage_release_pool()
  {
    SharedPointerReleasePool rp;
    std::shared_ptr<Number> num(new Number);
    rp.Append(num);

    {
      std::shared_ptr<int> ptr(new int(8));
      rp.Append(ptr);
    }

    {
      std::shared_ptr<Number> ptr(new Number(18));
      rp.Append(ptr);
    }

    std::this_thread::sleep_for(
      std::chrono::milliseconds(1200));

    {
      std::shared_ptr<int> ptr(new int(8));
      rp.Append(ptr);
    }
  }

} // namespace sharedpointer

CREATE_ELEMENT_WITH_CODE(SharedPointerUsage) {
  using namespace sharedpointer;
  shared_ptr_usage().join();
  usage_enable_shared_from_this().join();
}

CREATE_ELEMENT_WITH_CODE(WeakPointerUsage) {
  using namespace sharedpointer;
  usage_weak_ptr();
}

CREATE_ELEMENT_WITH_CODE(SharedPointerPoolUsage) {
  using namespace sharedpointer;
  usage_release_pool();
}
