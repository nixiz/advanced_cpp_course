#include <advanced_cpp_topics.h>
#include <iostream>
#include "inc/number.hpp"

using namespace std;

/*
  daha detayl� anlat�m ve �rnek i�in a�a��daki link'ten faydalan�labilir:
  http://www.thinkbottomup.com.au/site/blog/C%20%20_Mixins_-_Reuse_through_inheritance_is_good
*/
namespace mixin {
  namespace problem
  {

    // number s�n�f�m�z i�in bir interface tan�mlamas� yapm�� olal�m
    // number s�f�m�z�n da bu interface �zerinden t�redi�ini varsayal�m
    struct INumber {
      virtual ~INumber() {};
      virtual void set(int v) = 0;
      virtual int get() const = 0;
    };

    namespace approach_using_inheritance
    {
      // ve bu number interface'ine uygun olan s�n�flar i�in undo �zelli�i 
      // eklemek isteyelim. bunun i�in de a�a��daki abstract s�n�f� yazal�m
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

      // ayn� �ekilde redo yapabilmesi i�in gerekli ara abstract s�n�f� yazal�m
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

      // ger�eklemek istedi�imiz number s�n�f�n� undo �zelli�ine sahip olmas�
      // i�in IUndoableNumber'dan t�remesi yeterli, ancak hem undo hem de redo 
      // yapmak istedi�imiz zaman bunu ��zebiliriz??
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
      - base class kullan�m� ile bu ger�eklemeyi yapmaya �al��t���m�mzda ambiguity problemi
      ile kar��la��yoruz.
      - Ayn� �ekilde base class'lar�n �zerine kabuk yazarak ekstra virtual metodlar ile
      'vtable' kullam�n� daha da artt�rm�� oluyoruz, ki bu kodun i�letimi a��s�ndan �ok k�t�
      bir yakla��m.
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
      Composite strategy pattern uygulasak bile, undo ve redo metodlar�n� nas�l
      �a��raca��z? bir�ok i�lem yapt�ktan sonra bunu ��zebiliriz ancak yine de
      bir �ok heap allocation yapmak zorunda kalaca��z ve ayn� �ekilde yarat�lan
      s�n�flar�n ownership'likleri kimde olacak veya lifecycle'� kim y�netecek
      problemleri olacakt�r.
      Bu yakla��m da sorunu tam anlam�yla ��zmeyecektir.
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
      Bu y�ntemde, amac�m�z�n number s�n�f�na undo ve redo �zelliklerinin getirilmesi
      oldu�unu hat�rlayal�m ve bunu bir �ekilde ��zmemiz gerekti�ini d���nelim.
      Bu problemi ��zmek i�in de t�me var�m yerine tam tersten d���nelim; yani t�mden
      var�m y�ntemini uygulayal�m.
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

      // yukar�da da bahsetti�im gibi derived class'lar� base class'lara y�nlenmek yerine, 
      // tam tersine base class'dan derived'lar� ger�ekleyelim. (bu ne bi�im bir c�mle oldu?!?)
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

      // ve redo �zelli�ini de undoable s�n�f�ndan t�reyerek ekleyelim.
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
      bu �ekilde, bir �ekilde number temel s�n�f�m�za undo ve redo �zelliklerini katt�k!!!
      ama bunu yaparken hi� de generik bir yakla��m sergilemedik ve ba�ka tipteki s�n�flar
      i�in undo/redo �zelli�ini katmam�z imkans�z oldu!!!
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

ELEMENT_CODE(MixinTemplateExampleProblem) {
  using namespace mixin::problem;
  approach_using_inheritance::Demonstrate();

  approach_using_composition_strategy_pattern::Demonstrate();

  approach_reversed_inheritance::Demonstrate();
}

ELEMENT_CODE(MixinTemplateExample) {
  mixin::solution::ReUndoableNumber mynum;
  mynum.set(42); mynum.set(84);
  cout << mynum.get() << '\n';  // 84
  mynum.undo();
  cout << mynum.get() << '\n';  // 42
  mynum.redo();
  cout << mynum.get() << '\n';  // back to 84
}
