#include <advanced_cpp_topics.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <future>

namespace StaticUsage {

  struct CountedObject {
    static CountedObject GetInstance()
    {
      // With C++11 static member initializations are thread safe
      static CountedObject instance;
      return instance;
    }
  private:
    CountedObject()
    {
      std::cout << "CountedObject() thread: " << std::this_thread::get_id() << "\n";
    }
  };
}

// https://gcc.godbolt.org/z/G19zvG6YP
ELEMENT_CODE(StaticUsageExample) 
{
  using namespace StaticUsage;
  std::promise<void> wait_flag;
  auto watier = wait_flag.get_future();

  std::thread t1([] (auto wait_flag) {
    wait_flag.set_value();
    CountedObject::GetInstance();
  }, std::move(wait_flag));
  watier.wait();
  CountedObject::GetInstance();
  t1.join();
}