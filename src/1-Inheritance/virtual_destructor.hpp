#pragma once
#include <string.h>
#include <playground_organizer.hpp>
#include <iostream>
#include <vector>
#include "inc/number.hpp"

namespace virtualdestructor {

  namespace problem {
    class Object {
    public:
      Object(const char *name) : _name(name) {
        printf("\nobject created: %s", _name.c_str());
      }
      ~Object() {
        printf("\nobject destructed: %s", _name.c_str());
      }
    private:
      std::string _name;
    };
  }

  namespace solution {
    class Object {
    public:
      Object(const char *name) : _name(name) {
        printf("\nobject created: %s", _name.c_str());
      }
      virtual ~Object() {
        printf("\nobject destructed: %s", _name.c_str());
      }
    private:
      std::string _name;
    };
  }

  template <class Object>
  class DerivedFromObject : public Object {
    Number *_nameSize;
  public:
    DerivedFromObject(const char *name = "DerivedFromObject") : Object(name)
    {
      printf("\nDerivedFromObject created: %s", name);
      // ! gcc has a bug for decltype of private member !
#ifdef MSBUILD
      _nameSize = new Number(static_cast<decltype(Number::n)>(strlen(name)));
#else
      _nameSize = new Number(strlen(name));
#endif
    }
    ~DerivedFromObject()
    {
      printf("\nDerivedFromObject destructed");
      delete _nameSize;
    }
  };

  template <class Object>
  inline void _usage() {
    std::vector<std::unique_ptr<Object>> objectArray;
    // populate vector
    for (size_t i = 0; i < 2; i++)
    {
      std::ostringstream ss;
      ss << "object[" << i << "]";
      objectArray.emplace_back(new DerivedFromObject<Object>(ss.str().c_str()));
    }
    // then clear.
    objectArray.clear();
  }

  void Problem() {
    _usage<problem::Object>();
  }

  void Solution() {
    _usage<solution::Object>();
  }
} // namespace virtualdestructor

CREATE_ELEMENT_WITH_CODE(VirtualDestructorUsage) {
  {
    virtualdestructor::Problem();
  }
  {
    virtualdestructor::Solution();
  }
}