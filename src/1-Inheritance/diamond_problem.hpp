#pragma once
#include <playground_organizer.hpp>
#include <iostream>
#include <set>

#include "template_inheritance_usage.hpp"

namespace diamondproblem {
  //namespace tt = TemplateInheritanceUsage::TemplateSolution;
  using TemplateInheritanceUsage::TemplateSolution::Singleton;

  class Object;
  class ICTerraFrameworkObjectManager : public Singleton<ICTerraFrameworkObjectManager> {
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
    explicit Object(const char *name = "ICTerra Object") : _name(name) {
      ICTerraFrameworkObjectManager::getInstance().RegisterObject(this);
    }
    virtual ~Object() {
      ICTerraFrameworkObjectManager::getInstance().UnregisterObject(this);
    }
    std::string getName() const {
      return _name;
    }
  private:
    std::string _name;
  };

  //
  void ICTerraFrameworkObjectManager::printObjects() const {
    printf("\ntotal alive objects: %I64d", _totalObjects);
    for (Object* obj : registeredObjects)
    {
      printf("\n%s", obj->getName().c_str());
    }
  }


  namespace problem {

    class ComponentA : public Object {
    public: ComponentA() : Object("ICTerra ComponentA") { }
    };

    class ComponentB : public Object {
    public: ComponentB() : Object("ICTerra ComponentB") { }
    };
#pragma warning( push )
#pragma warning( disable : 4584) // disable compiler warning for multiple base class inheritance
    class CompoisiteComponent : public Object, public ComponentA {
    public: CompoisiteComponent() : Object("ICTerra CompoisiteComponent") { }
    };
#pragma warning( pop ) 

  }  // namespace problem

  namespace solution {

    class ComponentA : virtual public Object {
    public: ComponentA() : Object("ICTerra ComponentA") { }
    };

    class ComponentB : virtual public Object {
    public: ComponentB() : Object("ICTerra ComponentB") { }
    };

    class CompoisiteComponent : public ComponentB, public ComponentA {
    public: CompoisiteComponent() : Object("ICTerra CompoisiteComponent") { }
    };

  }  // namespace problem

  void PrintObjects() {
    ICTerraFrameworkObjectManager::getInstance().printObjects();
  }
}  // namespace diamondproblem

CREATE_ELEMENT_WITH_CODE(DiamondProblemExample) {
  namespace dp = diamondproblem;
  {
    dp::problem::ComponentA a;
    dp::problem::ComponentB b;
    dp::problem::CompoisiteComponent cc;
    dp::PrintObjects();
  }
  dp::PrintObjects();

  {
    dp::solution::ComponentA a;
    dp::solution::ComponentB b;
    dp::solution::CompoisiteComponent cc;
    dp::PrintObjects();
  }
  dp::PrintObjects();
}