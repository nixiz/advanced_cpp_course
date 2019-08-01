#pragma once
#include <playground_organizer.hpp>
#include <iostream>
#include "inc/number.hpp"

using namespace std;

namespace staticassert {

  namespace definition {
#pragma warning( push )
#pragma warning( disable : 4101) // disable compiler warning for unused variable unnamed
    /*
    The simplest solution to compile-time assertions (Van Horn 1997), and one that works in C as well as in
    C++, relies on the fact that a zero-length array is illegal.
    */
#define STATIC_CHECK(expr) { char unnamed[(expr) ? 1 : 0]; }

    template <class To, class From>
    To safe_reinterpret_cast(From from)
    {
      // if size of 'From' type are larger 'To', the compiler complains that you are trying to
      // create an array of length zero.
      STATIC_CHECK(sizeof(From) <= sizeof(To));
      return reinterpret_cast<To>(from);
    }

    void willsuccess() {
      alignas(int) char stack_buffer[sizeof(int)];
      int *val = new(stack_buffer) int(3);
      char *bytes = safe_reinterpret_cast<char*>(val);
      printf("\ninteger bytes : %d %d %d %d\n", bytes[0], bytes[1], bytes[2], bytes[3]);
    }

    /*
    The problem with this approach is that the error message you receive is not terribly informative.
    "Cannot create array of size zero" does not suggest "Type char is too narrow to hold a pointer."
    It is very hard to provide customized error messages portably. Error messages have no rules that
    they must obey; it's all up to the compiler. For instance, if the error refers to an undefined variable,
    the name of that variable does not necessarily appear in the error message
    */
    void willfail() {
      alignas(double) char stack_buffer[sizeof(double)];
      double* somePointer = new (stack_buffer) double(3.14);
      // uncommenting below line will fail compile by giving static assertation error! "error C2466: cannot allocate an array of constant size 0"
      // char c = safe_reinterpret_cast<char>(somePointer);
    }
#pragma warning( pop ) 
  } // namespace definition 

  namespace usingtemplates {

    template<bool> struct CompileTimeError;
    // no implementation for false case to create compile time assertation
    template<> struct CompileTimeError<true> {};

#ifdef STATIC_CHECK
#undef STATIC_CHECK
#endif
    // redefine static check macro
#define STATIC_CHECK(expr) (CompileTimeError<(expr) != 0>())

    template <class To, class From>
    To safe_reinterpret_cast(From from)
    {
      STATIC_CHECK(sizeof(From) <= sizeof(To));
      return reinterpret_cast<To>(from);
    }
    /*
    CompileTimeError is a template taking a nontype parameter (a Boolean constant). Compile-
    TimeError is defined only for the true value of the Boolean constant. If you try to instantiate
    CompileTimeError<false>, the compiler utters a message such as "Undefined specialization
    CompileTimeError<false>." This message is a slightly better hint that the error is intentional and not
    a compiler or a program bug.
    */
    void willfail() {
      alignas(double) char stack_buffer[sizeof(double)];
      double* somePointer = new (stack_buffer) double(3.14);
      // uncommenting below line will fail compile by giving static assertation error! "error C2466: cannot allocate an array of constant size 0"
      //char c = safe_reinterpret_cast<char>(somePointer);
    }

  } // namespace usingtemplates

  namespace witherrormsg {

    // added ctor with any type
    template<bool> struct CompileTimeChecker {
      CompileTimeChecker(...) { }
    };
    // in here we specialized for false type and not implemented ctor for false 
    template<> struct CompileTimeChecker<false> {};

#ifdef STATIC_CHECK
#undef STATIC_CHECK
#endif
    // redefine static check macro
#define STATIC_CHECK(expr, msg) \
{ \
class ERROR_##msg {}; \
(void)sizeof(CompileTimeChecker<(expr) != 0>((ERROR_##msg()))); \
}

    /*
    After macro preprocessing, the code of safe_reinterpret_cast expands to the following:
    template <class To, class From>
    To safe_reinterpret_cast(From from)
    {
      {
        class ERROR_Destination_Type_Too_Narrow {};
        (void)sizeof(
          CompileTimeChecker<(sizeof(From) <= sizeof(To))>(
            ERROR_Destination_Type_Too_Narrow()));
      }
      return reinterpret_cast<To>(from);
    }
    */
    template <class To, class From>
    To safe_reinterpret_cast(From from)
    {
      STATIC_CHECK(sizeof(From) <= sizeof(To), Destination_Type_Too_Narrow);
      return reinterpret_cast<To>(from);
    }

    /*
    Now here's the trick. The CompileTimeChecker<true> specialization has a constructor that accepts
    anything; it's an ellipsis function. This means that if the compile-time expression checked evaluates to
    true, the resulting program is valid. If the comparison between sizes evaluates to false, a compile-time
    error occurs: The compiler cannot find a conversion from an
    ERROR_Destination_Type_Too_Narrow to a CompileTimeChecker<false>. And the nicest
    thing of all is that a decent compiler outputs an error message such as "Error: Cannot convert
    ERROR_Destination_Type_Too_Narrow to CompileTimeChecker <false>."
    */
    void willfail() {
      alignas(double) char stack_buffer[sizeof(double)];
      double* somePointer = new (stack_buffer) double(3.14);
      // uncommenting below line will fail compile by giving static assertation error! "error C2466: cannot allocate an array of constant size 0"
      //char c = safe_reinterpret_cast<char>(somePointer);
    }

  } // namespace usingtemplates

} // namespace staticassert

CREATE_ELEMENT_WITH_CODE(TemplateStaticAssert) {
  namespace sa = staticassert;

  sa::definition::willsuccess();  
  sa::definition::willfail();

  sa::usingtemplates::willfail();
  sa::witherrormsg::willfail();
}
