#pragma once

#include <playground_organizer.hpp>
#include <sstream>
#include <iostream>

namespace FriendUsageRealWorld {

  class Rectangle {
    int width, height;
    friend class RectangleTest;
  public:
    Rectangle() {}
    Rectangle(int x, int y) : width(x), height(y) {}
    int area() const { return width * height; }
    friend std::ostream& operator << (std::ostream& os, Rectangle const & rec);
  };

  std::ostream& operator << (std::ostream& os, Rectangle const & rec)
  {
    os << "width: " << rec.width << "\theight: " << rec.height << std::endl << "area of rectangle: " << rec.area();
    return os;
  }

  class RectangleTest
  {
  public:
    bool Test()
    {
      Rectangle rect {10,5};
      if (rect.width != 10 || rect.height != 5) return false;
      auto res = rect.area();
      if (res != 50) return false;
      return true;
    }
  };

} // namespace FriendUsageRealWorld

CREATE_ELEMENT_WITH_CODE(FriendUsageExample) {
  namespace fu = FriendUsageRealWorld;
  fu::Rectangle rec(3, 5);
  std::cout << rec << std::endl;
  fu::RectangleTest test;
  test.Test();
}
