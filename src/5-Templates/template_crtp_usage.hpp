#pragma once
#include <playground_organizer.hpp>
#include <iostream>

namespace traits
{
  namespace problem {
    // common abstract base class GeoObj for geometric objects
    class GeoObj {
    public:
      GeoObj() = default;
      // draw geometric object:
      virtual void draw() const = 0;
      virtual int number_of_draws() const = 0;
    protected:
      mutable int _number_of_draws = 0;
      //...
    };

    // concrete geometric object class Circle
    // - derived from GeoObj
    class Circle : public GeoObj {
    public:
      void draw() const override { std::cout << __FUNCTION__ << std::endl; _number_of_draws++; }

      int number_of_draws() const override {
        return _number_of_draws;
      }
      //...
    };

    // concrete geometric object class Line
    // - derived from GeoObj
    class Line : public GeoObj {
    public:
      void draw() const override { std::cout << __FUNCTION__ << std::endl; _number_of_draws++; }
      int number_of_draws() const override {
        return _number_of_draws;
      }
      //...
    };

    // draw any GeoObj
    void myDraw(GeoObj const& obj)
    {
      obj.draw();            // call draw() according to type of object
    }

    // draw heterogeneous collection of GeoObjs
    void drawElems(std::vector<GeoObj*> const& elems)
    {
      for (unsigned i = 0; i<elems.size(); ++i) {
        elems[i]->draw();  // call draw() according to type of element
      }
    }

    // ana problem, number_of_draws() eklediðimizde bunu crtp ile nasýl yapabiliriz ve 
    // her derive edilen tip için number_of_draws deðerini nasýl tek kerede hesaplayabiliriz.
    // mesela Circle c instance'ý daha toplam hesaplanmadan destruct olsaydý nasýl hesaplayacaktýk??
    void usage()
    {
      Line l;
      Circle c;

      myDraw(l);
      myDraw(c);
      {
        Circle c1, c2;
        myDraw(c1);
        myDraw(c2);
      }

      std::vector<GeoObj*> coll;  // heterogeneous collection
      coll.push_back(&l);         // insert line
      coll.push_back(&c);         // insert circle
      drawElems(coll);            // draw different kinds of GeoObjs

      std::cout << l.number_of_draws() << " line(s) drawn" << std::endl;
      std::cout << c.number_of_draws() /*+ c1.number_of_draws() + c2.number_of_draws() */
                << " circle(s) drawn" << std::endl;
      std::cout << "what should be total draws for circle?!?" << std::endl;
    }

  } // namespace problem

  namespace solution {
    
    // forward decleration of GeoObjTraits base trait class
    template <typename derived>
    struct GeoObjTraits {
      static int _num_of_draws;
    };
    template <typename derived>
    int GeoObjTraits<derived>::_num_of_draws = 0;

    template <typename derived>
    class GeoObj {
    public:
      GeoObj() = default;
      void draw() const {
        static_cast<derived const*>(this)->draw_impl();
        GeoObjTraits<derived>::_num_of_draws++;
      }

      int number_of_draws() const {
        return GeoObjTraits<derived>::_num_of_draws;
      }
      //...
    };

    class Circle : public GeoObj<Circle> {
    public:
      void draw_impl() const { std::cout << __FUNCTION__ << std::endl; }
      //...
    };

    class Line : public GeoObj<Line> {
    public:
      void draw_impl() const { std::cout << __FUNCTION__ << std::endl; }
      //...
    };

    template <typename GeoObj>
    void myDraw(GeoObj const& obj)
    {
      obj.draw();    // call draw() according to type of object
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
      Circle c;

      myDraw(l);        // myDraw<Line>(GeoObj&) => Line::draw()
      myDraw(c);        // myDraw<Circle>(GeoObj&) => Circle::draw()
      {
        Circle c1, c2;
        myDraw(c1);
        myDraw(c2);
      }

      std::vector<Line> coll;     // OK: homogeneous collection possible or use std::variant for heterogeneous container
      coll.push_back(l);          // insert line
      drawElems(coll);            // draw all lines

      std::cout << l.number_of_draws() << " line(s) drawn" << std::endl;
      std::cout << c.number_of_draws() << " circle(s) drawn" << std::endl;
    }
    
  } // namespace solution

} // namespace traits

CREATE_ELEMENT_WITH_CODE(TemplateCrtpUsage) {
  using namespace traits;
  problem::usage();
  solution::usage();
}


