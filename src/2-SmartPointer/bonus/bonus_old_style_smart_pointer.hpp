#pragma once
#include <playground_organizer.hpp>
#include <vector>
#include "inc/number.hpp"
#include "handle.h"
#include "shared.h"

namespace smartpointer {

  struct SharedNumber : Number, ICT::SimpleShared {
    SharedNumber(int initial) : Number(initial) { }
    ~SharedNumber() {
      std::cout << __FUNCTION__ << " should be called only once!" << std::endl;
    }
  };
  typedef ICT::Handle<SharedNumber> SharedNumberHandle;
} // namespace smartpointer

CREATE_ELEMENT_WITH_CODE(OldStyleSmartPointer) {
  using namespace smartpointer;
  SharedNumberHandle sharedNumberPtr = new SharedNumber(10);

  // create vector with 3 elements of shared number pointer
  std::vector<SharedNumberHandle> sharedNumberVector{ sharedNumberPtr, sharedNumberPtr, sharedNumberPtr };

  for (const auto& number : sharedNumberVector) {
    // be sure that every item of vector use the same instance of number
    _ASSERT(number.get() == sharedNumberPtr.get());
  }

  sharedNumberVector.clear();
  std::cout << "Total shared count: " << sharedNumberPtr._ptr->__getRef() << std::endl;
}