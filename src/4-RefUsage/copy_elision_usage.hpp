#pragma once
#ifndef COPY_ELISION_HPP_ 
#define COPY_ELISION_HPP_

#include <playground_organizer.hpp>
#include <iostream>

using namespace std;
namespace copyelision {
  class BigObject {
  public:
    BigObject() {
      cout << "constructor. " << endl;
    }
    ~BigObject() {
      cout << "destructor." << endl;
    }
    BigObject(const BigObject&) {
      cout << "copy constructor." << endl;
    }
    BigObject(BigObject&&) {
      cout << "move constructor" << endl;
    }
  };

  BigObject fooMove() {
    BigObject localObj;
    return std::move(localObj);
  }

  BigObject&& fooRefRef() {
    BigObject localObj;
    return std::move(localObj);
  }

  BigObject fooValue(int i) {
    BigObject localObj, anotherObj;
    if (i > 2) {
      return localObj;
    }
    else {
      return anotherObj;
    }
  }

  BigObject&& fooRefRef(int i) {
    BigObject localObj, anotherObj;
    if (i > 2) {
      return std::move(localObj);
    }
    else {
      return std::move(anotherObj);
    }
  }
}

CREATE_ELEMENT_WITH_CODE(CopyElisionReturnByValue) {
  {
    class BigObject {
    public:
      BigObject() {
        cout << "constructor. " << endl;
      }
      ~BigObject() {
        cout << "destructor." << endl;
      }
      BigObject(const BigObject&) {
        cout << "copy constructor." << endl;
      }
    };
    auto foo = []() -> BigObject {
      BigObject localObj;
      return localObj;
    };
    std::cout << "BigObject localObj = foo();" << std::endl << std::endl;
    BigObject localObj = foo();
    std::cout << "BigObject localObj = foo();       Done    " << std::endl;
  }

  {
    using namespace copyelision;
    std::cout << "BigObject localObj = fooMove();" << std::endl << std::endl;
    BigObject localObj = fooMove();
    std::cout << "BigObject localObj = fooMove();   Done    " << std::endl << std::endl;
  }

  {
    class BigObject {
    public:
      BigObject() {
        cout << "constructor. " << endl;
      }
      ~BigObject() {
        cout << "destructor." << endl;
      }
      BigObject(const BigObject&) {
        cout << "copy constructor." << endl;
      }
    };
    auto fooValue = [](int i) -> BigObject {
      BigObject localObj, anotherObj;
      if (i > 2) {
        return localObj;
      }
      else {
        return anotherObj;
      }
    };
    std::cout << "BigObject localObj = fooValue(1);" << std::endl << std::endl;
    BigObject localObj = fooValue(1);
    std::cout << "BigObject localObj = fooValue(1); Done    " << std::endl << std::endl;
  }
}

CREATE_ELEMENT_WITH_CODE(CopyElisionReturnByRefRef) {
  using namespace copyelision;
  {
    std::cout << "BigObject localObj = fooRefRef();" << std::endl << std::endl;
    BigObject localObj = fooRefRef();
    std::cout << "BigObject localObj = fooRefRef();    Done   " << std::endl << std::endl;
  }
  {
    std::cout << "BigObject localObj = fooRefRef(1);" << std::endl << std::endl;
    BigObject localObj = fooRefRef(1);
    std::cout << "BigObject localObj = fooRefRef(1);   Done   " << std::endl << std::endl;
  }
}
#endif // !COPY_ELISION_HPP_ 