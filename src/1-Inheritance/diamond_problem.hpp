#pragma once
#include <playground_organizer.hpp>
#include <iostream>
#include <set>

#include "template_inheritance_usage.hpp"

namespace diamondproblem {
  //namespace tt = TemplateInheritanceUsage::TemplateSolution;
  using TemplateInheritanceUsage::TemplateSolution::Singleton;

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
    explicit Object(const char *name = "STM Object") : _name(name) {
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
    printf("\ntotal alive objects: %I64d", _totalObjects);
    for (Object* obj : registeredObjects)
    {
      printf("\n%s", obj->getName().c_str());
    }
  }


  namespace problem {

    class ComponentA : public Object {
    public: ComponentA() : Object("STM ComponentA") { }
    };

    class ComponentB : public Object {
    public: ComponentB() : Object("STM ComponentB") { }
    };
#pragma warning( push )
#pragma warning( disable : 4584) // disable compiler warning for multiple base class inheritance
    class CompositeComponent : public Object, public ComponentA {
    public: CompositeComponent() : Object("STM CompositeComponent") { }
    };
#pragma warning( pop ) 

  }  // namespace problem

  namespace solution {

    class ComponentA : virtual public Object {
    public: ComponentA() : Object("STM ComponentA") { }
    };

    class ComponentB : virtual public Object {
    public: ComponentB() : Object("STM ComponentB") { }
    };

    class CompositeComponent : public ComponentB, public ComponentA {
    public: CompositeComponent() : Object("STM CompositeComponent") { }
    };

  }  // namespace problem

  void PrintObjects() {
    STMFrameworkObjectManager::getInstance().printObjects();
  }
}  // namespace diamondproblem

CREATE_ELEMENT_WITH_CODE(DiamondProblemExample) {
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