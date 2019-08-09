#pragma once
#include <playground_organizer.hpp>
#include <iostream>
#include "inc/number.hpp"

using namespace std;

/*
  daha detaylý anlatým ve örnek için aþaðýdaki link'ten faydalanýlabilir:
  http://www.thinkbottomup.com.au/site/blog/C%20%20_Mixins_-_Reuse_through_inheritance_is_good
*/
namespace mixin {
  namespace problem
  {

    // number sýnýfýmýz için bir interface tanýmlamasý yapmýþ olalým
    // number sýfýmýzýn da bu interface üzerinden türediðini varsayalým
    struct INumber {
      virtual ~INumber() {};
      virtual void set(int v) = 0;
      virtual int get() const = 0;
    };

    namespace approach_using_inheritance 
    {
      // ve bu number interface'ine uygun olan sýnýflar için undo özelliði 
      // eklemek isteyelim. bunun için de aþaðýdaki abstract sýnýfý yazalým
      class IUndoableNumber : public INumber {
      protected:
        int _number_old;
      public:
        virtual ~IUndoableNumber() {}
        virtual int get() const = 0;

        void set(int v) override {
          _number_old = get();
          setNewNumber(v);
        }

        void undo() {
          setNewNumber(_number_old);
        }
      protected:
        virtual void setNewNumber(int v) = 0;
      };

      // ayný þekilde redo yapabilmesi için gerekli ara abstract sýnýfý yazalým
      class IRedoableNumber : public INumber {
      protected:
        int _number_after;
      public:
        virtual ~IRedoableNumber() {}
        virtual int get() const = 0;

        void set(int v) override {
          _number_after = get();
          setNewNumber(v);
        }

        void redo() {
          setNewNumber(_number_after);
        }
      protected:
        virtual void setNewNumber(int v) = 0;
      };

      // gerçeklemek istediðimiz number sýnýfýný undo özelliðine sahip olmasý
      // için IUndoableNumber'dan türemesi yeterli, ancak hem undo hem de redo 
      // yapmak istediðimiz zaman bunu çözebiliriz??
      class UndoableNumber : public IUndoableNumber/*, public IRedoableNumber*/ {
      public:
        UndoableNumber(int initial = 0) : _num(initial) { }
        int get() const override { return _num; }
      private:
        void setNewNumber(int v) override { _num = v; }
        int _num;
      };
      /*
      Buradaki problemler:
      - base class kullanýmý ile bu gerçeklemeyi yapmaya çalýþtýðýmýmzda ambiguity problemi
      ile karþýlaþýyoruz.
      - Ayný þekilde base class'larýn üzerine kabuk yazarak ekstra virtual metodlar ile
      'vtable' kullamýný daha da arttýrmýþ oluyoruz, ki bu kodun iþletimi açýsýndan çok kötü
      bir yaklaþým.
      */
      void Demonstrate() {
        UndoableNumber n(5);
        n.set(10);
        std::cout << "number: " << n.get() << std::endl;
        n.undo();
        std::cout << "number: " << n.get() << std::endl;
      }

    } // namespace approach_using_inheritance

    namespace approach_using_composition_strategy_pattern 
    {

      class IUndoableNumber : public INumber {
      protected:
        int _number_old;
        INumber *number_ptr;
      public:
        IUndoableNumber(INumber *numberptr) : number_ptr(numberptr) { }
        virtual ~IUndoableNumber() { if (number_ptr) delete number_ptr; }

        int get() const override {
          if (number_ptr)
            return number_ptr->get();
          return _number_old;
        }

        void set(int v) override {
          _number_old = v;
          number_ptr->set(v);
        }

        void undo() {
          number_ptr->set(_number_old);
        }
      };

      class IRedoableNumber : public INumber {
      protected:
        int _number_after;
        INumber *number_ptr;
      public:
        IRedoableNumber(INumber *numberptr) : number_ptr(numberptr) { }
        virtual ~IRedoableNumber() { if (number_ptr) delete number_ptr; }

        int get() const override {
          if (number_ptr)
            return number_ptr->get();
          return _number_after;
        }

        void set(int v) override {
          _number_after = v;
          if (number_ptr)
            number_ptr->set(v);
        }

        void redo() {
          number_ptr->set(_number_after);
        }
      };

      class Number : public INumber {
      public:
        Number(int initial = 0) : _num(initial) { }
        int get() const override { return _num; }
        void set(int v) override { _num = v; }
      private:
        int _num;
      };

      /*
      Composite strategy pattern uygulasak bile, undo ve redo metodlarýný nasýl
      çaðýracaðýz? birçok iþlem yaptýktan sonra bunu çözebiliriz ancak yine de
      bir çok heap allocation yapmak zorunda kalacaðýz ve ayný þekilde yaratýlan
      sýnýflarýn ownership'likleri kimde olacak veya lifecycle'ý kim yönetecek
      problemleri olacaktýr.
      Bu yaklaþým da sorunu tam anlamýyla çözmeyecektir.
      */
      void Demonstrate() {
        INumber * number =
          new IRedoableNumber(
            new IUndoableNumber(
              new Number(5)));

        number->set(10);
        std::cout << "number: " << number->get() << std::endl;
        //number->undo(); ??
        std::cout << "number: " << number->get() << std::endl;
      }

    } // namespace approach_using_composition_strategy_pattern

    namespace approach_reversed_inheritance {
      /*
      Bu yöntemde, amacýmýzýn number sýnýfýna undo ve redo özelliklerinin getirilmesi
      olduðunu hatýrlayalým ve bunu bir þekilde çözmemiz gerektiðini düþünelim.
      Bu problemi çözmek için de tüme varým yerine tam tersten düþünelim; yani tümden
      varým yöntemini uygulayalým.
      */
      class Number : public INumber {
      public:
        Number(int initial = 0) : _num(initial) { }
        virtual ~Number() { }

        virtual int get() const { return _num; }
        virtual void set(int v) { _num = v; }
      private:
        int _num;
      };

      // yukarýda da bahsettiðim gibi derived class'larý base class'lara yönlenmek yerine, 
      // tam tersine base class'dan derived'larý gerçekleyelim. (bu ne biçim bir cümle oldu?!?)
      class IUndoableNumber : public Number {
      protected:
        int _number_old = 0;
      public:
        virtual ~IUndoableNumber() {}

        virtual void set(int v) {
          _number_old = get();
          Number::set(v);
        }

        void undo() {
          Number::set(_number_old);
        }
      };

      // ve redo özelliðini de undoable sýnýfýndan türeyerek ekleyelim.
      class IReundoableNumber : public IUndoableNumber {
      protected:
        int _number_after = 0;
      public:
        virtual ~IReundoableNumber() {}

        virtual void set(int v) {
          _number_after = v;
          IUndoableNumber::set(v);
        }

        void redo() {
          IUndoableNumber::set(_number_after);
        }
      };

      /*
      bu þekilde, bir þekilde number temel sýnýfýmýza undo ve redo özelliklerini kattýk!!!
      ama bunu yaparken hiç de generik bir yaklaþým sergilemedik ve baþka tipteki sýnýflar
      için undo/redo özelliðini katmamýz imkansýz oldu!!!
      */
      void Demonstrate() {
        IReundoableNumber num;
        num.set(10);
        std::cout << "number: " << num.get() << std::endl;
        num.undo();
        std::cout << "number: " << num.get() << std::endl;
        num.redo();
        std::cout << "number: " << num.get() << std::endl;
      }
    } // namespace approach_reversed_inheritance
  } // namespace problem

  namespace solution 
  {

    template <typename Base>
    struct Undoable : public Base {
      typedef typename Base::value_type T;
      T before;
      void set(T v) { before = Base::get(); Base::set(v); }
      void undo() { Base::set(before); }
    };

    template <typename Base>
    struct Redoable : public Base {
      typedef typename Base::value_type T;
      T after;
      void set(T v) { after = v; Base::set(v); }
      void redo() { Base::set(after); }
    };

    typedef Redoable< Undoable<Number> > ReUndoableNumber;
  } // namespace solution
} // namespace mixin 

CREATE_ELEMENT_WITH_CODE(MixinTemplateExampleProblem) {
  using namespace mixin::problem;
  approach_using_inheritance::Demonstrate();

  approach_using_composition_strategy_pattern::Demonstrate();

  approach_reversed_inheritance::Demonstrate();
}

CREATE_ELEMENT_WITH_CODE(MixinTemplateExample) {
  mixin::solution::ReUndoableNumber mynum;
  mynum.set(42); mynum.set(84);
  cout << mynum.get() << '\n';  // 84
  mynum.undo();
  cout << mynum.get() << '\n';  // 42
  mynum.redo();
  cout << mynum.get() << '\n';  // back to 84
}
