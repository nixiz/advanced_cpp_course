#include <advanced_cpp_topics.h>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <algorithm>

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
    virtual ~Shape() = default;

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

using namespace InheritanceUsage;

void getCircleFromShape(const Shape & shape)
{
  auto radius = static_cast<const Circle&>(shape).getRadius();
}

void getCircleFromShape(Shape* shape)
{
  auto radius = static_cast<Circle*>(shape)->getRadius();
}

ELEMENT_CODE(InheritanceUsageExample) 
{
  std::vector<Shape*> shapes {
    new Circle(5),
    new Rectangle(3, 5),
    new Triangle(12, 3, 30)
  };

  auto total_area = std::reduce(begin(shapes),
                                end(shapes),
                                0.f,
                                [](float area, Shape* shape) {
                                  return area + shape->getArea();
                                });
  std::cout << "total area of all shapes are " << std::fixed << total_area << "\n";

}