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
    ReUndoableNumberThrows number_will_throw;
    number_will_throw.set(10); number_will_throw.undo(); number_will_throw.redo();
  }
  catch (const std::exception&)
  {
    std::cout << "exception catched as assumed.." << std::endl;
  }

}

