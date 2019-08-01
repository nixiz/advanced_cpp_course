#pragma once
#include <playground_organizer.hpp>
#include <iostream>
#include <typeinfo>

namespace templatedecltype {
  template <typename T1, typename T2>
  auto Sum(T1&& a, T2&& b) -> decltype(a + b) {
    return std::forward<T1>(a) + std::forward<T2>(b);
  }
}

CREATE_ELEMENT_WITH_CODE(TemplateDecltypeUsage) {
  int    i = templatedecltype::Sum(1, 2);
  double d = templatedecltype::Sum(1.0, 2.1);
  auto   a = templatedecltype::Sum(1, 2.1);
  auto   b = templatedecltype::Sum(1.1, 2);

  std::cout << typeid(a).name() << " " << typeid(b).name();
  _ASSERT(typeid(a) == typeid(b));
  _ASSERT(typeid(a) == typeid(double));
}
