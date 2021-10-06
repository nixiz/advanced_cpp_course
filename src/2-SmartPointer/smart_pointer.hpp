#pragma once
#include <playground_organizer.hpp>
#include <string.h>
#include "inc/number.hpp"

namespace smartpointer {

  // Number Factory for new pointer allocation
  Number *CreateNumber(int initial)
  {
    return new Number(initial);
  }

  void badcalc() { throw new std::runtime_error("sorry my bad :("); }

  namespace problem {
    void bad() {
      Number * num = new Number(10);
      // .. some calculation logic here
      badcalc();
      delete num;
    }
  }

  namespace halfsolution {
    // RAII helper class for deleting allocated pointers
    class PointerDeleteHelper {
    public:
      PointerDeleteHelper(Number *numberpointer) : number_pointer(numberpointer) {}

      ~PointerDeleteHelper() {
        if (number_pointer)
          delete number_pointer;
      }
    private:
      Number *number_pointer;
    };

    void willwork() {
      Number * num = new Number(10);
      PointerDeleteHelper pointerHolder(num);
      // .. some calculation logic here
      badcalc();
      delete num; // never delete pointer twice!
    }

    void willNOTwork() {
      Number * num = new Number(10);
      PointerDeleteHelper pointerHolder(num);
      // after some code:
      // re-initialize pointer
      delete num;
      num = CreateNumber(1);
      // what will happen on exit this scope?
    }
  } // namespace halfsolution 

  namespace solution {
    void willwork() {
      std::unique_ptr<Number> num(new Number());
      // after some code:
      // re-initialize pointer
      num.reset(CreateNumber(1));
      badcalc();
    }
  }
} // namespace smartpointer

CREATE_ELEMENT_WITH_CODE(SmartPointerUsage) {
  using namespace smartpointer;
  try
  {
    problem::bad();
  }
  catch (...) {}

  try
  {
    halfsolution::willwork();

    halfsolution::willNOTwork();
  }
  catch (...) {}

  try
  {
    solution::willwork();
  }
  catch (...) {}
}