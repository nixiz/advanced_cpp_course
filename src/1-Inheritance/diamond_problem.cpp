#include <advanced_cpp_topics.h>
#include <iostream>
#include <set>

namespace diamondproblem {
  namespace problem {
    class Shape {
    public:
      explicit Shape(const char* n)
        : name(n) 
      { 
        std::cout << "I'm a(n) " << name << "\n";
      }

      void printArea() const {
        std::cout << "area of "
          << name << ": "
          << this->getArea() << "\n";
      }

      virtual float getArea() const = 0;
    private:
      const char* name;
    };

    class Rectangle : public Shape {
    public:
      Rectangle(float w, float h)
        : Shape("Rectangle")
        , width(w)
        , height(h)
      { }

      float getArea() const override {
        return width * height;
      }
    protected:
      float width;
      float height;
    };

    class Triangle : public Shape {
    public:
      Triangle(float b, float h, float y)
        : Shape("Triangle")
        , base(b), height(h), y_angle(y)
      { }

      float getArea() const override {
        return (base * height) / 2.f;
      }
    protected:
      float base, height, y_angle;
    };

    class ComplexShape 
      : public Triangle
      , public Rectangle 
    {
    public:
      ComplexShape(Rectangle r, Triangle t)
        //: Shape("ComplexShape")
        : Rectangle(r)
        , Triangle(t)
      { }

      float getArea() const override {
        return width * width;
      }
    };

    void demonstrate()
    {
      ComplexShape cs(Rectangle(5, 3), Triangle(5, 2, 12));
      //cs.printArea();
      std::cout << "area of complex shape is " << cs.getArea() << "\n";

      //also cannot use polymorphism
      //Shape* cs_ptr = new ComplexShape(Rectangle(5, 3), Triangle(5, 2, 12));
    }
  } // namespace problem


  namespace solved {
    class Shape {
    public:
      explicit Shape(const char* n)
        : name(n) 
      {
        std::cout << "I'm a(n) " << name << "\n";
      }
      ~Shape() = default;

      void printArea() const {
        std::cout << "area of "
          << name << ": "
          << this->getArea() << "\n";
      }

      virtual float getArea() const = 0;
    private:
      const char* name;
    };

    class Rectangle : virtual public Shape {
    public:
      Rectangle(float w, float h)
        : Shape("Rectangle")
        , width(w)
        , height(h)
      { }

      float getArea() const override {
        return width * height;
      }
    protected:
      float width;
      float height;
    };
 
    class Triangle : virtual public Shape {
    public:
      Triangle(float b, float h, float y)
        : Shape("Triangle")
        , base(b), height(h), y_angle(y)
      { }

      float getArea() const override {
        return (base * height) / 2.f;
      }
    protected:
      float base, height, y_angle;
    };

    class ComplexShape 
      : public Triangle
      , public Rectangle 
    {
    public:
      ComplexShape(Rectangle r, Triangle t)
        : Shape("Square")
        , Rectangle(r)
        , Triangle(t)
      { }

      float getArea() const override {
        return width * width;
      }
    };

    void demonstrate()
    {
      ComplexShape cs(Rectangle(5, 3), Triangle(5, 2, 12));
      cs.printArea();
      Shape* cs_ptr = new ComplexShape(Rectangle(5, 3), Triangle(5, 2, 12));
      cs_ptr->printArea();

    }
  } // namespace problem
}  // namespace diamondproblem

ELEMENT_CODE(DiamondProblemExample) {
  using namespace diamondproblem;
  problem::demonstrate();
  solved::demonstrate();
}