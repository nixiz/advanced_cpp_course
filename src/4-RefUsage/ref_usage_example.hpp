#pragma once
#include <playground_organizer.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include <typeinfo>       // operator typeid

namespace RefUsage {

  int func(int i) {
    return i++;
  }

  int func_ref(int& i) {
    return i++;
  }

  int func_ref_const(int const& i) {
    //return i++; compiler error: expression must be modifiable lvalue
    return i;
  }

  int func_ref_const_cast(int const& i) {
    return const_cast<int&>(i)++;
  }

  int func_ref_ref(int&& i) {
    return i++;
  }
  //template <typename Fnc, typename... Args>
  //void CallWithDescription(Fnc function, Args&& ...args) {
  //  std::cout << "Calling function: " << typeid(function).name() << std::endl;
  //  function(std::forward<Args>(args)...);
  //}
}

CREATE_ELEMENT_WITH_CODE(RefUsageExample) {
  using namespace RefUsage;

  func(10);
  //func_ref(10); compile error C2664: must be lvalue
  func_ref_const(10);
  func_ref_ref(10);
  func_ref_const_cast(10);
}