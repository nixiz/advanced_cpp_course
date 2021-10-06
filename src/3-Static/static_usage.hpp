#pragma once
#include <playground_organizer.hpp>
#include <iostream>
#include <sstream>
#include <thread>

namespace StaticUsage {

  struct CountedObject {
    int data;

    static CountedObject CreateNew()
    {
      std::cout << "called from thread: " << std::this_thread::get_id() << std::endl << std::flush;
      CountedObject::_id++; // increment totat creation count
      return CountedObject();
    }

    static unsigned int TotalCreationCount() {
      return CountedObject::_id;
    }

  private:
    CountedObject() = default;
    static unsigned int _id;
  };
  //static
  unsigned int CountedObject::_id = 0;
}

CREATE_ELEMENT_WITH_CODE(StaticUsageExample) {
  using namespace StaticUsage;
  std::thread t1([]() {
    for (size_t i = 0; i < 10; i++)
    {
      CountedObject::CreateNew();
    }
  });
  for (size_t i = 0; i < 10; i++)
  {
    CountedObject::CreateNew();
  }
  t1.join();
  printf("Total creation count: %d", CountedObject::TotalCreationCount());
}