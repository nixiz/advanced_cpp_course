#pragma once
#include <memory>
#include <atomic>
#include <playground_organizer.hpp> // for _ASSERT macro

//
// Base classes for reference counted types. The ICT::Handle
// template can be used for smart pointers to types derived from these
// bases.
//
// ICT::SimpleShared
// =====================
//
// A non thread-safe base class for reference-counted types.
//
// ICT::Shared
// ===============
//
// A thread-safe base class for reference-counted types.
//
// Usage:
// ===============
// struct SmartObject : public SimpleShared {
//   void foo() {}
// };
// 
// void foo() {
//   ICT::Handle<SmartObject> SmartObjectPtr = new SmartObject();
//   SmartObjectPtr->foo();
//   SmartObject *ptr = SmartObjectPtr.get();
// }
// 
namespace ICT
{

  class SimpleShared {
  public:

    SimpleShared();
    SimpleShared(const SimpleShared&);

    virtual ~SimpleShared()
    {
    }

    SimpleShared& operator=(const SimpleShared&)
    {
      return *this;
    }

    void __incRef()
    {
      _ASSERT(_ref >= 0);
      ++_ref;
    }

    void __decRef()
    {
      _ASSERT(_ref > 0);
      if (--_ref == 0)
      {
        if (!_noDelete)
        {
          delete this;
        }
      }
    }

    int __getRef() const
    {
      return _ref;
    }

    void __setNoDelete(bool b)
    {
      _noDelete = b;
    }

  private:

    int _ref;
    bool _noDelete;
  };

  class  Shared {
  public:

    static const unsigned char NoDelete;

    Shared();
    Shared(const Shared&);

    virtual ~Shared()
    {
    }

    Shared& operator=(const Shared&)
    {
      return *this;
    }

    virtual void __incRef();
    virtual void __decRef();
    virtual int __getRef() const;
    virtual void __setNoDelete(bool);

    void __setFlag(unsigned char flag)
    {
      _flags |= flag;
    }

    void __clearFlag(unsigned char flag)
    {
      _flags &= ~flag;
    }

    bool __hasFlag(unsigned char flag)
    {
      return (_flags & flag) > 0;
    }

  protected:

    std::atomic<int> _ref;
    unsigned char _flags;
  };

}
