#include <advanced_cpp_topics.h>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>
#include <functional>

namespace InheritanceUsage 
{
  class Shape {
  public:
    explicit Shape(const std::string& name_)
      : name(name_)
      , area()
    {
      std::cout << "my name is " << name << "\n";
    }

    const char* getName() const {
      return name.c_str();
    }

    float getArea() const {
      return area;
    }
  protected:
    float area;
  private:
    const std::string name;
  };

  class Circle : public Shape {
  public:
    explicit Circle(float r)
      : Shape("Circle")
      , radius(r) 
    {
      area = radius * pi_constant * pi_constant;
    }
    
    // circle specific properties
    float getRadius() const {
      return radius;
    }
  private:
    float radius;
    const float pi_constant = 3.14f;
  };

  class Rectangle : public Shape {
  public:
    Rectangle(float w, float h)
      : Shape("Rectangle")
      , width(w)
      , height(h)
    {
      area = width * height;
    }

  // rect shape properties
  private:
    float width;
    float height;
  };

  class Triangle : public Shape {
  public:
    Triangle(float b, float h, float y)
      : Shape("Rectangle")
      , base(b)
      , height(h)
      , y_angle(y)
    {
      area = (base * height) / 2.f;
    }
  // triangle properties
  private:
    float base;
    float height;
    float y_angle;
  };

} // namespace InheritanceUsage


ELEMENT_CODE(InheritanceUsageExample) 
{
  using namespace InheritanceUsage;
  std::vector<std::unique_ptr<Shape>> shapes;
  shapes.emplace_back(new Circle(5));
  shapes.emplace_back(new Rectangle(3, 5));
  shapes.emplace_back(new Triangle(12, 3, 30));

  auto total_area = std::accumulate(begin(shapes),
                                    end(shapes),
                                    0.f,
                                    [](float area, auto& shape) {
                                      return area + shape->getArea();
                                    });
  std::cout << "total area of all shapes are " << std::fixed << total_area << "\n";
}


namespace AccessControl
{
  class base
  {
  public:
    base() {
      static auto print_once = [&] {
        std::cout << "class can access all of its members "
          "no matter access control\n";
        std::cout << "private var: " << private_var << "\n";
        std::cout << "protected var: " << protected_var << "\n";
        std::cout << "public var: " << public_var << "\n";
        return true;
      } ();
    }
  public:
    int public_var{ 1 };
  protected:
    int protected_var{ 2 };
  private:
    int private_var{ 3 };
  };

  class public_derived
    : public base { };

  class protected_derived
    : protected base { };

  class private_derived
    : private base {
  public:
    private_derived() {
      std::cout << "derived classes can access protected and "
        "public base variables inside of class\n";
      // std::cout << "private var: " << private_var << "\n";
      std::cout << "base protected var: " << protected_var << "\n";
      std::cout << "base public var: " << public_var << "\n";
    }
  };
}

ELEMENT_CODE(InheritanceAccessControl)
{
  using namespace AccessControl;
  base b;
  public_derived pd;
  protected_derived pro_d;
  private_derived priv_d;
  std::cout << "access to base class variables from "
               "derived class instances depends on inheritance type\n";

  std::cout << "when an inheritance marked as protected; all public "
               "members of base goes to protected inside of derived class\n";

  std::cout << "when an inheritance made as private then members of base "
               "goes to private inside of derived class\n";
//pd.public_var;
//pd.protected_var;
//pd.private_var;
//
//pro_d.public_var;
//pro_d.protected_var;
//pro_d.private_var;
//  
//priv_d.public_var;
//priv_d.protected_var;
//priv_d.private_var;
}


namespace VirtualFunctions
{
  class Shape {
  public:
    // ordinary member function
    void printArea() const {
      std::cout << "area of " 
        << this->getName() << ": " 
        << this->getArea() << "\n";
    }
    // virtual function with default behavior
    virtual const char* getName() const {
      return "[undefined shape]";
    }
    // pure virtual function
    virtual float getArea() const = 0;
  };

  class Circle : public Shape {
  public:
    explicit Circle(float r)
      : radius(r) { }

    float getArea() const override {
      return radius * pi_constant * pi_constant;
    }

    const char* getName() const override {
      return "Circle";
    }
  private:
    float radius;
    const float pi_constant = 3.14f;
  };

  class Rectangle : public Shape {
  public:
    Rectangle(float w, float h)
      : width(w)
      , height(h) { }

    float getArea() const override {
      return width * height;
    }

    const char* getName() const override {
      return "Rectangle";
    }
  private:
    float width;
    float height;
  };

  class Triangle : public Shape {
  public:
    Triangle(float b, float h, float y)
      : base(b) , height(h) , y_angle(y) 
    { }

    float getArea() const override {
      return (base * height) / 2.f;
    }
  private:
    float base, height, y_angle;
  };
}


ELEMENT_CODE(InheritanceVirtualFunctions)
{
  using namespace VirtualFunctions;
  std::vector<std::unique_ptr<Shape>> shapes;
  shapes.emplace_back(new Circle(5));
  shapes.emplace_back(new Rectangle(3, 5));
  shapes.emplace_back(new Triangle(12, 3, 30));

  std::for_each(begin(shapes),end(shapes), 
                std::mem_fn(&Shape::printArea));
}
