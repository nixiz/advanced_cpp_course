#pragma once
#include <playground_organizer.hpp>
#include <iostream>
#include <string>

using namespace std;

namespace TemplateInheritanceUsage {

  namespace Problem {
    class SingletonClass {
    private:
      SingletonClass()
      {
        cout << "ctor:SingletonClass" << endl;
      }
    public:
      static SingletonClass& getInstance()
      {
        static SingletonClass instance;
        return instance;
      }
      void foo() {}
      void bar() {}
    };
  } // namespace Problem

  namespace TemplateSolution {
    template<typename T> // Template Singleton Implementer
    class Singleton {
    protected:
      Singleton() = default;
      Singleton(const Singleton&) = delete;
      Singleton& operator=(const Singleton&) = delete;
      virtual ~Singleton() = default;
    public:
      static T& getInstance()
      {
        static T instance;
        return instance;
      }
    };

    class MySingletonClassA : public Singleton<MySingletonClassA> {
      friend class Singleton<MySingletonClassA>;
      MySingletonClassA() { std::cout << "MySingletonClassA::MySingletonClassA()" << std::endl; }
    public:
      void foo() {}
      void bar() {}
    };

    class MySingletonClassB : public Singleton<MySingletonClassB> {
    public:
      void foo() {}
      void bar() {}
    };
  }

  namespace TemplateSolutionPhase2 {
    template<typename T> // Template Singleton Implementer
    class Singleton {
    protected:
      Singleton() = default;
      Singleton(const Singleton&) = delete;
      Singleton& operator=(const Singleton&) = delete;
      virtual ~Singleton() = default;
    public:
      template <typename ...Args>
      static T& getInstance(Args&&... args)
      {
        static T instance(std::forward<Args>(args)...);
        return instance;
      }
    };

    class MySingletonClassA : public Singleton<MySingletonClassA> {
      friend class Singleton<MySingletonClassA>;
      MySingletonClassA(int i) { std::cout << "MySingletonClassA::MySingletonClassA()" << std::endl; }
    public:
      void foo() {}
      void bar() {}
    };

    class MySingletonClassB : public Singleton<MySingletonClassB> {
    public:
      void foo() {}
      void bar() {}
    };
  }

} // namespace TemplateInheritanceUsage

CREATE_ELEMENT_WITH_CODE(TemplateInheritanceUsageExample) {
  {
    namespace ft = TemplateInheritanceUsage::TemplateSolution;
    std::cout << "Address of MySingletonClassA: " << &ft::MySingletonClassA::getInstance() << std::endl;
    std::cout << "Address of MySingletonClassA: " << &ft::MySingletonClassA::getInstance() << std::endl;
    ft::MySingletonClassB::getInstance().bar();
  }

  {
    namespace ft = TemplateInheritanceUsage::TemplateSolutionPhase2;
    std::cout << "Address of MySingletonClassA: " << &ft::MySingletonClassA::getInstance(10) << std::endl;
    std::cout << "Address of MySingletonClassA: " << &ft::MySingletonClassA::getInstance(5) << std::endl;
    ft::MySingletonClassB::getInstance().bar();
  }
}

#include "src/0-Types/no_destruct.h"

namespace googleway {

  class MySingletonClassA {
  public:
    void foo() {}
    void bar() {}
  };

  class MySingletonClassB {
  public:
    MySingletonClassB(std::string msg)
    {
      std::cout << msg << "\n";
    }
    ~MySingletonClassB()
    {
      _ASSERT_EXPR(false, "Dtor called");
    }
    void foo() {}
    void bar() {}
  };

  void Usage() {
    chromium::base::NoDestructor<std::string> s("this will not destruct!");
    chromium::base::NoDestructor<MySingletonClassA> a;
    chromium::base::NoDestructor<MySingletonClassB> b("this will not destruct!");
    a->foo();
    b->bar();
  }
}

CREATE_ELEMENT_WITH_CODE(GoogleNoDestructor) {
  using namespace googleway;
  chromium::base::NoDestructor<std::string> s("this will not destruct!");
  chromium::base::NoDestructor<MySingletonClassA> a;
  chromium::base::NoDestructor<MySingletonClassB> b("this will not destruct!");
  a->foo();
  b->bar();
  std::cout << *s << std::endl;
}
