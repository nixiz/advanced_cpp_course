#pragma once
#include <playground_organizer.hpp>
#include <iostream>
#include "src/0-Types/number.hpp"

namespace policy {

  /// number s�n�f� �rne�i i�in bakt���m�zda undo veya redo yaparken
  /// baz� k�s�tlamalar koymak istersek, bunu yapabilmek i�in bu i�lemleri
  /// bizim i�in ger�ekle�tiren s�n�flara bu k�s�tlamalar� ger�ekle�tirecek
  /// kodlar yazmam�z gerekecektir. Bunu yaparken bazen ks�tlamak isteyebilir,
  /// bazen de k�s�tlama koymak istemeyebiliriz. Peki bunu nas�l generic bir
  /// �ekilde yapabiliriz? Bu yetenekleri ilgili undo ve redo yapan s�n�flara
  /// policy (hareket tarz�) eklememiz en iyi y�ntem olacakt�r. 
  /// Policy detayl� anlat�m i�in a�a��daki kitaptan anlat�m yap:
  /// "Andrei Alexandrescu - Modern C++ design - generic programming and design patterns applied (2001)"
  template <typename T>
  struct ThrowIfZero {
    class SetZeroException : public std::exception {};

    static void check(T* t) {
      if (t->get() == 0) throw SetZeroException();
    }
  };

  template <typename T>
  struct AllowAny {
    static void check(T* t) { }
  };

  /*
   * no need to redefine number class 
  class Number {
  public:
    typedef int value_type;
    Number(int initial = 0) : _num(initial) { }
    virtual ~Number() { }

    int get() const { return _num; }
    void set(int v) { _num = v; }
  private:
    int _num;
  };
  */

  template <typename BASE, template <typename> typename CheckerPolicy = policy::ThrowIfZero>
  struct Undoable : public BASE, public CheckerPolicy<BASE> {
    typedef typename BASE::value_type value_type;

    void set(value_type v) {
      before = BASE::get();
      BASE::set(v);
    }

    void undo() {
      CheckerPolicy<BASE>::check(this);
      BASE::set(before);
    }

  protected:
    value_type before;
  };

  template <typename BASE, template <typename> typename CheckerPolicy = policy::AllowAny>
  struct Redoable : public BASE, public CheckerPolicy<BASE> {
    typedef typename BASE::value_type value_type;

    void set(value_type v) {
      after = v;
      BASE::set(v);
    }
    void redo() {
      CheckerPolicy<BASE>::check(this);
      BASE::set(after);
    }

  protected:
    value_type after;
  };

}

CREATE_ELEMENT_WITH_CODE(TemplatePolicies) {
  using namespace policy;
  typedef Redoable< Undoable<Number> >                      ReUndoableNumberWorks;
  typedef Redoable< Undoable<Number>, policy::ThrowIfZero > ReUndoableNumberThrows;

  // this will not throw because we accept any value on redo
  ReUndoableNumberWorks number;
  number.set(10); number.undo(); number.redo();

  try
  {
    // but this will throw as we gave policy "throw if zero"
    ReUndoableNumberThrows numberthrows;
    numberthrows.set(10); numberthrows.undo(); numberthrows.redo();
  }
  catch (const std::exception&)
  {
    // for debugging in catch block
    __nop();
  }

}

