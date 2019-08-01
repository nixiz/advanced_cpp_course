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

  namespace NewSingletonDesign
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
		  static StaticObjCreator _creator(pointee, std::forward<Args>(args)... );
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

	  Type * operator ->()
	  {
		  return &**this;
	  }

	  const Type* operator ->() const
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
    /*
      Burada bir kaç tane problem var:
        1.  Singleton<T> sınıfından kalıtım yapmak güzel bir çözüm değil.
            Polymorphic yapılarda veya çoklu kalıtımın mümkün olmadığı yerlerde uygulanamaz.
        2.  Aynı şekilde yukarıdaki gibi kullanım olduğunda sınıfların Ctor'unu gizlemek gerekmektedir
            Bu her zaman yapabileceğimiz bir şey değil; örneğin Qt içerisindeki sınıfların default ctor'unun
            her zaman public olması gerekmektedir.
        3.  Default Ctor'u olmayan sınıflar için Singleton<T>::getInstance() metodu kullanılması
            kullanışlı olmayacaktır. Yukarıdaki örnekte de görüleceği gibi "getInstance()" metodunu
            her çağırışımızda ilgili sınıfın Ctor'unda yer alan parametreleri vermemiz gerekecektir
            ki bu hiç de güzel ve kullanışlı bir yöntem değil.
      Yukarıdaki problemlerin çözümü için yeni ve farklı bir generik singleton tasarımı düşünmemiz gerekmektedir.
      Aynı şekilde yeni tasarımda singleton yapmak istediğimiz sınıfı, istersek singleton olmadan da kullanabilmeliyiz.
      Örnek olarak 'ClassA' sınıfını hem singleton hem de başka bir yerde normal bir instance olarak kullanmak isteyebiliriz.
      Kullanım şekli kabaca aşağıdaki gibi bir tasarıma ihtiyacımız var:
        ClassA instance(param1, ..., paramN); // normal instance kullanımı
        file1.cpp :
        Singleton<ClassA> singleton_instance(param1, ..., paramN) // singleton kullanımı. yaratılan nesne 0xBABA adresinde yer alıyor olsun
        file2.cpp :
        Singleton<ClassA> singleton_instance; // buradaki nesne de aynı adresteki singleton ClassA sınıfını kullanacaktır

        veya yaratmak istediğimiz yerde aşağıdaki gibi yaratıp
        Singleton<ClassA>::createInstance(param1, ..., paramN);
        kullandığımız yerlerde ise klasik erişim şekliyle erişebiliriz
        Singleton<ClassA>::getInstance()->foo();

      Bu tasarımın son hali yukarıda yer almaktadır.
    */
  }

  TemplateInheritanceUsage::NewSingletonDesign::usage();
  
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

CREATE_ELEMENT_WITH_CODE(GoogleNoDestructor) {
  using namespace googleway;
  chromium::base::NoDestructor<std::string> s("this will not destruct!");
  chromium::base::NoDestructor<MySingletonClassA> a;
  chromium::base::NoDestructor<MySingletonClassB> b("this will not destruct!");
  a->foo();
  b->bar();
  std::cout << *s << std::endl;
}
