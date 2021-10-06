#include <advanced_cpp_topics.h>
#include <iostream>
#include <set>

namespace diamondproblem {
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
    
  class Object;
  class STMFrameworkObjectManager : public Singleton<STMFrameworkObjectManager> {
    friend class Object;
  public:
    void printObjects() const;
  private:
    void RegisterObject(Object *obj) {
      _totalObjects++;
      registeredObjects.insert(obj);
    }

    void UnregisterObject(Object *obj) {
      _totalObjects--;
      registeredObjects.erase(obj);
    }
  private:
    std::set<Object*> registeredObjects;
    unsigned long long _totalObjects;
  };

  class Object {
  public:
    explicit Object(const char *name = "Object") : _name(name) {
      STMFrameworkObjectManager::getInstance().RegisterObject(this);
    }
    virtual ~Object() {
      STMFrameworkObjectManager::getInstance().UnregisterObject(this);
    }
    std::string getName() const {
      return _name;
    }
  private:
    std::string _name;
  };

  //
  void STMFrameworkObjectManager::printObjects() const {
    printf("\ntotal alive objects: %llu", _totalObjects);
    for (Object* obj : registeredObjects)
    {
      printf("\n%s", obj->getName().c_str());
    }
  }


  namespace problem {

    class ComponentA : public Object {
    public: ComponentA() : Object("ComponentA") { }
    };

    class ComponentB : public Object {
    public: ComponentB() : Object("ComponentB") { }
    };
#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable : 4584) // disable compiler warning for multiple base class inheritance
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Winaccessible-base"
#endif
    class CompositeComponent : public Object, public ComponentA {
    public: CompositeComponent() : Object("CompositeComponent") { }
    };
#ifdef _WIN32
#pragma warning( pop ) 
#else
#pragma GCC diagnostic pop
#endif

  }  // namespace problem

  namespace solution {

    class ComponentA : virtual public Object {
    public: ComponentA() : Object("ComponentA") { }
    };

    class ComponentB : virtual public Object {
    public: ComponentB() : Object("ComponentB") { }
    };

    class CompositeComponent : public ComponentB, public ComponentA {
    public: CompositeComponent() : Object("CompositeComponent") { }
    };

  }  // namespace problem

  void PrintObjects() {
    STMFrameworkObjectManager::getInstance().printObjects();
  }
}  // namespace diamondproblem

ELEMENT_CODE(DiamondProblemExample) {
  namespace dp = diamondproblem;
  {
    dp::problem::ComponentA a;
    dp::problem::ComponentB b;
    dp::problem::CompositeComponent cc;
    dp::PrintObjects();
  }
  dp::PrintObjects();

  {
    dp::solution::ComponentA a;
    dp::solution::ComponentB b;
    dp::solution::CompositeComponent cc;
    dp::PrintObjects();
  }
  dp::PrintObjects();
}