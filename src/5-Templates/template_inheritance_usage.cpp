#include <advanced_cpp_topics.h>
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
        // thread safety in initialization of static variable guaranteed with C++11 standard
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

  namespace VariadicTemplateSolution {

    namespace oldstyle
    {
      /*
        gereksinim:
        Singleton olarak kullanacağımız her sınıf defaut ctor'a sahip olmak zorunda değil.
        Bazı sınıfların ctor'ları parametre almak zorunda olabilir. bu nedenle singleton
        bu sınıfları singleton olarak yaratacaksak, başka bir çözüm bulmamız lazım.

        class ClassA : public Singleton<ClassA> {
        public:
          ClassA(int a, float b) ...
        };

        class ClassB : public Singleton<ClassB> {
        public:
          ClassB(string name) ...
        };

        Yukarıdaki gibi iki sınıfımızı singleton pattern için yarattığımız Singleton<T>
        sınıfı üzerinden kullanmak isteyelim

        template <class T>
        class Singleton {
          ...
          static T& getInstance(???) {
            static T inst(???); return inst;
          }
        };

        Singleton<T> sınıfı bu şartlar altında getInstance metoduna nasıl bir parametre almalı?
        Çözüm belirli bir parametre sayısı kadar getInstance metodunu overload ederek, muhtemel
        kullanılması beklenen sınıflar için kullanım imkanı sağlamak.
      */
      template<typename T>
      class Singleton {
      public:
        // peki buradaki Arg1 tipinin parametre signature'ı nasıl olmalı
        // const Arg1& | Arg1 * | ... ?
        template <typename Arg1>
        static T& getInstance(const Arg1& a1)
        {
          static T inst(a1); return inst;
        }

        template <typename Arg1, typename Arg2>
        static T& getInstance(const Arg1& a1, const Arg2& a2)
        {
          static T inst(a1, a2); return inst;
        }

        template <typename Arg1, typename Arg2, typename Arg3 /*, ... , typename ArgN*/>
        static T& getInstance(const Arg1& a1, const Arg2& a2, const Arg3& a3 /*, ... , const ArgN& aN*/)
        {
          static T inst(a1, a2, a3 /*, ..., aN*/); return inst;
        }

        // ... 
      protected:
        Singleton() = default;
        Singleton(const Singleton&) = delete;
        Singleton& operator=(const Singleton&) = delete;
        virtual ~Singleton() = default;
      };

    } // namespace oldstyle

    // variadic template implementation
    template<typename T>
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

  namespace CorrectSingletonDesign
  {
    template <class T>
    class Singleton {
    public:
      typedef typename std::remove_all_extents<T>::type Type;

      template <typename ...Args>
      Singleton(Args&& ...args)
      {
        struct StaticObjCreator
        {
          StaticObjCreator(Type*& obj_ptr, Args&& ...arguments)
          {
            assert(obj_ptr == nullptr);
            obj_ptr = new Type(std::forward<Args>(arguments)...);
          }
        };
        static StaticObjCreator _creator(pointee, std::forward<Args>(args)...);
        assert(pointee != nullptr);
      }

      template <typename ...Args>
      static void createInstance(Args&& ...args)
      {
        /*should be static ?*/
        Singleton _instance(std::forward<Args>(args)...);
        (void)_instance;
      }

      static Type *getInstance()
      {
        //if (pointee == nullptr) throw std::exception("");
        return pointee;
      }

      Type * operator->()
      {
        return &**this;
      }

      const Type* operator->() const
      {
        return &**this;
      }

      Type& operator *()
      {
        return *pointee;
      }

      const Type& operator *() const
      {
        return *pointee;
      }

    private:
      static Type *pointee;
    };
    template <typename T>
    typename Singleton<T>::Type * Singleton<T>::pointee = nullptr;

    class ClassA {
    public:
      ClassA() { std::cout << "ClassA::ClassA()" << std::endl; }
      void foo() {}
      void bar() {}
    };

    class ClassB {
    public:
      ClassB(int, double, std::string) {}
      void foo() {}
      void bar() {}
    };

    void usage()
    {
      Singleton<ClassA> a; // -> can be instantiated like a normal class
      Singleton<ClassA> aa; // -> aa will be referenced to a
      Singleton<ClassB> b(1, 3.14, "hello world!"); // -> 
      Singleton<ClassA>::getInstance()->foo(); //-> ClassA
      (*a).foo(); b->bar();   // -> and * operators both work
    }
  }

} // namespace TemplateInheritanceUsage

ELEMENT_CODE(TemplateInheritanceUsageExample) {
  {
    namespace ft = TemplateInheritanceUsage::TemplateSolution;
    std::cout << "Address of MySingletonClassA: " << &ft::MySingletonClassA::getInstance() << std::endl;
    std::cout << "Address of MySingletonClassA: " << &ft::MySingletonClassA::getInstance() << std::endl;
    ft::MySingletonClassB::getInstance().bar();
  }

  {
    namespace ft = TemplateInheritanceUsage::VariadicTemplateSolution;
    std::cout << "Address of MySingletonClassA: " << &ft::MySingletonClassA::getInstance(10) << std::endl;
    std::cout << "Address of MySingletonClassA: " << &ft::MySingletonClassA::getInstance(5) << std::endl;
    ft::MySingletonClassB::getInstance().bar();
  }

  TemplateInheritanceUsage::CorrectSingletonDesign::usage();

}

#include "inc/no_destruct.h"

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

ELEMENT_CODE(GoogleNoDestructor) {
  using namespace googleway;
  chromium::base::NoDestructor<std::string> s("this will not destruct!");
  chromium::base::NoDestructor<MySingletonClassA> a;
  chromium::base::NoDestructor<MySingletonClassB> b("this will not destruct!");
  a->foo();
  b->bar();
  std::cout << *s << std::endl;
}
