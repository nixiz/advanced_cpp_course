#include <advanced_cpp_topics.h>
#include <iostream>

namespace InheritanceUsage {
  class Polygon {
  protected:
    int width, height;
  public:
    void set_values(int a, int b) {
      width = a; height = b;
    }
  };

  class Rectangle : public Polygon {
  public:
    int area() {
      return width * height;
    }
  };

  class Triangle : public Polygon {
  public:
    int area() {
      return width * height / 2;
    }
  };

} // namespace InheritanceUsage

ELEMENT_CODE(InheritanceUsageExample) {
  using namespace InheritanceUsage;
  Rectangle rect;
  Triangle trgl;
  rect.set_values(4, 5);
  trgl.set_values(4, 5);
  std::cout << rect.area() << std::endl;
  std::cout << trgl.area() << std::endl;
}