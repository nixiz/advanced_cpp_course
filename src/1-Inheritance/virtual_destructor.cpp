#include <advanced_cpp_topics.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <numeric>
#include <bitset>
#include "inc/number.hpp"

namespace virtualdestructor {
  namespace problem {

    class Shape {
    public:
      Shape() {
        std::cout << "Shape Ctor\n";
      }
      ~Shape() {
        std::cout << "Shape ~Dtor\n";
      }

      void printArea() const {
        std::cout << "area of "
          << this->getName() << ": "
          << this->getArea() << "\n";
      }

      virtual const char* getName() const {
        return "[undefined shape]";
      }

      virtual float getArea() const = 0;
    };

    class Square : public Shape {
    public:
      explicit Square(int a)
        : width(a) 
      { 
        std::cout << "Square Ctor\n";
      }
      ~Square() {
        std::cout << "Square ~Dtor\n";
      }

      float getArea() const override {
        return (float)(width * width);
      }

      const char* getName() const override {
        return "Square";
      }
    private:
      int width;
    };

    //void printMemoryBlock(unsigned char *mem, size_t len)
    //{
    //  auto hex_string = [](unsigned char c) {
    //    std::stringstream ss;
    //    ss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    //    return ss.str();
    //  };
    //  auto dash_fold = [&](std::string a, unsigned char c) {
    //    return std::move(a) + '-' + hex_string(c);
    //  };
    //  std::string s = std::accumulate(std::next(mem + 1), mem + len,
    //                                  hex_string(mem[0]), // start with first element
    //                                  dash_fold);
    //  std::cout << "memory[" << len <<"]: " << s << "\n";
    //}

    //template <typename T>
    //void printMemoryBlock(T* ptr, size_t len = 0)
    //{
    //  printMemoryBlock(reinterpret_cast<unsigned char*>(ptr), len ? len : sizeof(T));
    //}

    void demonstrate()
    {
      Shape* s = new Square(0x01010101);
      s->printArea();
      delete s;
    }

  }

  namespace solution {

    class Shape {
    public:
      Shape() {
        std::cout << "Shape Ctor\n";
      }
      virtual ~Shape() {
        std::cout << "Shape ~Dtor\n";
      }

      void printArea() const {
        std::cout << "area of "
          << this->getName() << ": "
          << this->getArea() << "\n";
      }

      virtual const char* getName() const {
        return "[undefined shape]";
      }

      virtual float getArea() const = 0;
    };

    class Square : public Shape {
    public:
      explicit Square(int a)
        : width(a)
      {
        std::cout << "Square Ctor\n";
      }
      ~Square() {
        std::cout << "Square ~Dtor\n";
      }

      float getArea() const override {
        return (float)(width * width);
      }

      const char* getName() const override {
        return "Square";
      }
    private:
      int width;
    };

    void demonstrate()
    {
      Shape* s = new Square(0x01010101);
      s->printArea();
      delete s;
    }
  } // namespace solution

} // namespace virtualdestructor

ELEMENT_CODE(VirtualDestructorUsage) {
  using namespace virtualdestructor;
  {
    problem::demonstrate();
  }
  {
    solution::demonstrate();
  }
}