#pragma once
#include <playground_organizer.hpp>
#include <vector>
#include "src/0-Types/number.hpp"

namespace sharedpointer {

  void Usage() {
    std::shared_ptr<Number> sharedNumber = std::make_shared<Number>(10);
    using Vector = std::vector<std::shared_ptr<Number>>;
    // create vector with 3 elements of shared number pointer
    Vector sharedNumberVector{ sharedNumber, sharedNumber, sharedNumber };

    for (auto& number : sharedNumberVector) {
      // be sure that every item of vector use the same instance of number
      _ASSERT(number.use_count() == sharedNumberVector.size() + 1);
      _ASSERT(number->get() == sharedNumber->get());
    }
  }

  struct SharedNumber : Number, std::enable_shared_from_this<SharedNumber> {
    std::shared_ptr<SharedNumber> getptr() {
      return shared_from_this();
    }
    SharedNumber(int initial) : Number(initial) { }
  };

  void UsageSharedNumber() {
    std::shared_ptr<SharedNumber> sharedNumber = std::make_shared<SharedNumber>(10);
    using Vector = std::vector<std::shared_ptr<SharedNumber>>;
    // create vector with 3 elements of shared number pointer
    Vector sharedNumberVector{ sharedNumber->getptr(), sharedNumber->getptr(), sharedNumber->getptr() };

    for (auto& number : sharedNumberVector) {
      // be sure that every item of vector use the same instance of number
      _ASSERT(number.use_count() == sharedNumberVector.size() + 1);
      _ASSERT(number->get() == sharedNumber->get());
    }
  }

} // namespace sharedpointer

CREATE_ELEMENT_WITH_CODE(SharedPointerUsage) {
  using namespace sharedpointer;
  Usage();
  UsageSharedNumber();
}