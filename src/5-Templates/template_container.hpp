#pragma once
#include <playground_organizer.hpp>
#include <iostream>
#include <string>
#include <tuple>

namespace template_container {

  template <typename derived>
  class GeoObj {
  public:
    GeoObj() = default;
    std::string name() const {
      return static_cast<derived const*>(this)->getName();
    }
  };

  class Circle : public GeoObj<Circle> {
  public:
    std::string getName() const { return "circle"; }
  };

  class Line : public GeoObj<Line> {
  public:
    std::string getName() const { return "Line"; }
  };

  template <typename GeoObj>
  void printName(const GeoObj& v)
  {
    std::cout << v.name() << std::endl;
  }
  
  class GeoObjContainer
  {
  public:
    template<typename T>
    GeoObjContainer(const T& obj)
      : inner_(std::make_unique<Holder<T> >(obj)) { }
    
    const std::string name() const {
      return inner_->name();
    }

  private:
    struct HolderBase {
      virtual ~HolderBase() { }
      virtual const std::string name() const = 0;
    };

    template<typename T>
    struct Holder : public HolderBase {
      Holder(const T& obj) : obj_(obj) { }
      const std::string name() const override
      {
        return obj_.name();
      }
      T obj_;
    };
    std::unique_ptr<HolderBase> inner_;
  };

  void printName(const GeoObjContainer& v)
  {
    std::cout << v.name() << std::endl;
  }

} // namespace template_container 

CREATE_ELEMENT_WITH_CODE(TemplateContainerExample) {
  using namespace template_container;
  std::vector<GeoObjContainer*> vec;
  vec.push_back(new GeoObjContainer(Line{}));
  vec.push_back(new GeoObjContainer(Circle{}));
  GeoObjContainer l(Line{});
  printName(l);

  GeoObjContainer c(Circle{});
  printName(c);
}
