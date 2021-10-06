#include <advanced_cpp_topics.h>
#include <iostream>
#include "inc/number.hpp"

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
  struct AllowAny {
    static void check(T* t) { }
  };

  template <typename T>
  struct ThrowIfZero {
    class ValueZeroException : public std::exception {};

    static void check(T* t) {
      if (t->get() == 0) throw ValueZeroException();
    }
  };

  template <typename Base, template <typename> class CheckerPolicy = policy::ThrowIfZero>
  struct Undoable : public Base {
    typedef typename Base::value_type value_type;

    void set(value_type v) {
      before = Base::get();
      Base::set(v);
    }

    void undo() {
      CheckerPolicy<Base>::check(this);
      Base::set(before);
    }

  protected:
    value_type before;
  };

  template <typename Base, template <typename> class CheckerPolicy = policy::AllowAny>
  struct Redoable : public Base {
    typedef typename Base::value_type value_type;

    void set(value_type v) {
      after = v;
      Base::set(v);
    }
    void redo() {
      CheckerPolicy<Base>::check(this);
      Base::set(after);
    }

  protected:
    value_type after;
  };

}

ELEMENT_CODE(TemplatePolicies) {
  using namespace policy;
  typedef Redoable< Undoable<Number> >                      ReUndoableNumberWorks;
  typedef Redoable< Undoable<Number>, policy::ThrowIfZero > ReUndoableNumberThrows;

  // this will not throw because we accept any value on redo
  ReUndoableNumberWorks number;
  number.set(10); number.undo(); number.redo();

  try
  {
    // but this will throw as we gave policy "throw if zero"
    ReUndoableNumberThrows number_will_throw;
    number_will_throw.set(10); number_will_throw.undo(); number_will_throw.redo();
  }
  catch (const std::exception&)
  {
    std::cout << "exception catched as assumed.." << std::endl;
  }

}

