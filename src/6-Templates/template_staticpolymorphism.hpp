#pragma once

#include <playground_organizer.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>

namespace crtp {

  class Coord {
  private:
    int x, y;
  public:
    Coord(int i1, int i2) : x(i1), y(i2) {
    }
    friend Coord operator- (Coord const& c1, Coord const& c2) {
      return Coord(c1.x - c2.x, c1.y - c2.y);
    }
    Coord abs() {
      return Coord(std::abs(x), std::abs(y));
    }
  };

  namespace dynamicpolymorphism {

    // common abstract base class GeoObj for geometric objects
    class GeoObj {
    public:
      GeoObj() = default;
      // draw geometric object:
      virtual void draw() const = 0;
      // return center of gravity of geometric object:
      virtual Coord center_of_gravity() const = 0;
      //...
    };

    // concrete geometric object class Circle
    // - derived from GeoObj
    class Circle : public GeoObj {
    public:
      virtual void draw() const { std::cout << __FUNCTION__ << std::endl; }
      virtual Coord center_of_gravity() const { return Coord(10, 10); }
      //...
    };

    // concrete geometric object class Line
    // - derived from GeoObj
    class Line : public GeoObj {
    public:
      virtual void draw() const { std::cout << __FUNCTION__ << std::endl; }
      virtual Coord center_of_gravity() const { return Coord(5, 1); }
      //...
    };

    // draw any GeoObj
    void myDraw(GeoObj const& obj)
    {
      obj.draw();            // call draw() according to type of object
    }

    // process distance of center of gravity between two GeoObjs
    Coord distance(GeoObj const& x1, GeoObj const& x2)
    {
      Coord c = x1.center_of_gravity() - x2.center_of_gravity();
      return c.abs();        // return coordinates as absolute values
    }

    // draw heterogeneous collection of GeoObjs
    void drawElems(std::vector<GeoObj*> const& elems)
    {
      for (unsigned i = 0; i<elems.size(); ++i) {
        elems[i]->draw();  // call draw() according to type of element
      }
    }

    void usage()
    {
      Line l;
      Circle c, c1, c2;

      myDraw(l);            // myDraw(GeoObj&) => Line::draw()
      myDraw(c);            // myDraw(GeoObj&) => Circle::draw()

      distance(c1, c2);      // distance(GeoObj&,GeoObj&)
      distance(l, c);        // distance(GeoObj&,GeoObj&)

      std::vector<GeoObj*> coll;  // heterogeneous collection
      coll.push_back(&l);         // insert line
      coll.push_back(&c);         // insert circle
      drawElems(coll);            // draw different kinds of GeoObjs
    }

  } // namespace dynamicpolymorphism

  namespace staticpolymorphism {

    // concrete geometric object class Circle
    // - \bfseries not derived from any class
    class Circle {
    public:
      void draw() const { std::cout << __FUNCTION__ << std::endl; }
      Coord center_of_gravity() const { return Coord(5, 1); }
      //...
    };

    // concrete geometric object class Line
    // - \bfseries not derived from any class
    class Line {
    public:
      void draw() const { std::cout << __FUNCTION__ << std::endl; }
      Coord center_of_gravity() const { return Coord(5, 1); }
      //...
    };

    // draw any GeoObj
    template <typename GeoObj>
    void myDraw(GeoObj const& obj)
    {
      obj.draw();    // call draw() according to type of object
    }

    // process distance of center of gravity between two GeoObjs
    template <typename GeoObj1, typename GeoObj2>
    Coord distance(GeoObj1 const& x1, GeoObj2 const& x2)
    {
      Coord c = x1.center_of_gravity() - x2.center_of_gravity();
      return c.abs();  // return coordinates as absolute values
    }

    // draw homogeneous collection of GeoObjs
    template <typename GeoObj>
    void drawElems(std::vector<GeoObj> const& elems)
    {
      for (unsigned i = 0; i<elems.size(); ++i) {
        elems[i].draw();    // call draw() according to type of element
      }
    }

    void usage() {
      Line l;
      Circle c, c1, c2;

      myDraw(l);        // myDraw<Line>(GeoObj&) => Line::draw()
      myDraw(c);        // myDraw<Circle>(GeoObj&) => Circle::draw()

      distance(c1, c2);  // distance<Circle,Circle>(GeoObj1&,GeoObj2&)
      distance(l, c);    // distance<Line,Circle>(GeoObj1&,GeoObj2&)

    //std::vector<GeoObj*> coll;  // ERROR: no heterogeneous collection possible
      std::vector<Line> coll;     // OK: homogeneous collection possible
      coll.push_back(l);          // insert line
      drawElems(coll);            // draw all lines
    }

  } // namespace staticpolymorphism 
} // namespace crtp 


CREATE_ELEMENT_WITH_CODE(DynamicPolymorphismExample) {
  crtp::dynamicpolymorphism::usage();
}

CREATE_ELEMENT_WITH_CODE(StaticPolymorphismExample) {
  crtp::staticpolymorphism::usage();
}

