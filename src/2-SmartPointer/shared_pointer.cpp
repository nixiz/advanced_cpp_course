#include <advanced_cpp_topics.h>
#include <vector>
#include <thread>
#include <chrono>
#include <future>
#include "inc/number.hpp"

namespace sharedpointer {
  using namespace std::chrono_literals;
  using namespace std;

  void second_thread_func(std::shared_ptr<Number> num)
  {
    std::cout << "thread: [" << std::this_thread::get_id() << "] started\n";
    std::cout << "use count for shared_ptr<Number>: " << num.use_count() << std::endl;
    std::this_thread::sleep_for(2s);
  }

  void shared_ptr_usage()
  {
    std::shared_ptr<Number> sharedNumber{ new Number{10} };
    std::cout << "\nthread: [" << std::this_thread::get_id() << "] started\n";
    std::cout << "use count for shared_ptr<Number>: " << sharedNumber.use_count() << std::endl;

    std::thread(second_thread_func, sharedNumber).detach();
  }

  struct SharedNumber 
    : Number
    , std::enable_shared_from_this<SharedNumber> 
  {
    auto getPtr() {
      return shared_from_this();
    }

    explicit SharedNumber(int initial) 
      : Number(initial) { }
    ~SharedNumber() = default;

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

  void* number_ptr = NULL;
  void some_c_function_take_shptr(void* number_ptr_)
  {
    number_ptr = number_ptr_;
  }

  void* some_c_func_return_shptr() {
    return number_ptr;
  }

  void usage_enable_shared_from_this()
  {
    auto num = std::make_shared<SharedNumber>(10);
    std::cout
      << "thread: [" << std::this_thread::get_id()
      << "] use count for shared_ptr<SharedNumber>: "
      << num.use_count() << "\n";
    // pass number instance to c library
    some_c_function_take_shptr(num.get());

    auto waiter = std::async(launch::async,
      [] {
        // retreive number instance from c library
        void* ptr = some_c_func_return_shptr();
        auto num_ptr = reinterpret_cast<SharedNumber*>(ptr);
        auto num_shrd_ptr = num_ptr->getPtr();
        std::cout
          << "thread: [" << std::this_thread::get_id()
          << "] use count for shared_ptr<SharedNumber>: "
          << num_shrd_ptr.use_count() << "\n";
      });
    waiter.wait();
  }

  void second_thread_func_weak(std::weak_ptr<Number> num) {
    while (true) {
      if (num.expired()) break;
      auto shared_num = num.lock();
      std::cout
        << "thread: [" << std::this_thread::get_id()
        << "] use count for shared_ptr<Number>: "
        << shared_num.use_count() << "\n";
      std::this_thread::sleep_for(1s);
    }
    std::cout << "thread will end here.. use count: " << num.use_count() << "\n";
  }

  void usage_weak_ptr() {
    std::future<void> waiter;
    {
      auto sharedNumber = std::make_shared<Number>(10);
      waiter = std::async(launch::async, second_thread_func_weak,
                          std::weak_ptr<Number>(sharedNumber));
      std::this_thread::sleep_for(200ms);
    }
    waiter.wait();
  }

  // fake garbage collector using shared pointers
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

ELEMENT_CODE(SharedPointerUsage) {
  using namespace sharedpointer;
  //shared_ptr_usage().join();
  usage_enable_shared_from_this();
}

ELEMENT_CODE(WeakPointerUsage) {
  using namespace sharedpointer;
  usage_weak_ptr();
}

ELEMENT_CODE(SharedPointerPoolUsage) {
  using namespace sharedpointer;
  usage_release_pool();
}
