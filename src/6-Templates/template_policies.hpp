#pragma once
#include <playground_organizer.hpp>
#include <iostream>
#include "src/0-Types/number.hpp"

namespace policy {

  /// number sýnýfý örneði için baktýðýmýzda undo veya redo yaparken
  /// bazý kýsýtlamalar koymak istersek, bunu yapabilmek için bu iþlemleri
  /// bizim için gerçekleþtiren sýnýflara bu kýsýtlamalarý gerçekleþtirecek
  /// kodlar yazmamýz gerekecektir. Bunu yaparken bazen ksýtlamak isteyebilir,
  /// bazen de kýsýtlama koymak istemeyebiliriz. Peki bunu nasýl generic bir
  /// þekilde yapabiliriz? Bu yetenekleri ilgili undo ve redo yapan sýnýflara
  /// policy (hareket tarzý) eklememiz en iyi yöntem olacaktýr. 
  /// Policy detaylý anlatým için aþaðýdaki kitaptan anlatým yap:
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

