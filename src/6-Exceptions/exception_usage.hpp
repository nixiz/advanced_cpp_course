#pragma once
#include <playground_organizer.hpp>
#include <exception>
#include <iostream>
#include <sstream>

namespace exceptions {

  namespace ordering {

    struct A {
      int n;
      A(int n = 0) : n(n) { std::cout << "A(" << n << ") constructed successfully\n"; }
      ~A() { std::cout << "A(" << n << ") destroyed\n"; }
    };

    int foo()
    {
      throw std::runtime_error("error");
    }

    struct B {
      A a1, a2, a3;
      B() try : a1(1), a2(foo()), a3(3) {
        std::cout << "B constructed successfully\n";
      }
      catch (...) {
        std::cout << "B::B() exiting with exception\n";
      }
      ~B() { std::cout << "B destroyed\n"; }
    };

    struct C : A, B {
      C() try {
        std::cout << "C::C() completed successfully\n";
      }
      catch (...) {
        std::cout << "C::C() exiting with exception\n";
      }
      ~C() { std::cout << "C destroyed\n"; }
    };


    void usage() try
    {
      // creates the A base subobject
      // creates the a1 member of B
      // fails to create the a2 member of B
      // unwinding destroys the a1 member of B
      // unwinding destroys the A base subobject
      C c;
    }
    catch (const std::exception& e)
    {
      std::cout << "usage() failed to create C with: " << e.what();
    }
  } // namespace ordering

  namespace custom {

    class CustomException : public std::exception {
    public:
      // delegating constructors available in C++11
      CustomException(int line, const char* const file) : CustomException(line, file, nullptr) { }
      
      CustomException(int line, const char* const file, const char* const message)
      {
        std::ostringstream exceptionStream;
        exceptionStream
          << "ICTerra Exception Occured: "
          << "\nin line: " << line
          << "\nfile: " << file;
        if (message)
        {
          exceptionStream
            << "\nmessage: " << message;
        }
        wellformed_what = exceptionStream.str();
      }
      virtual ~CustomException() throw () {}

      const char* what() const throw ()
      {
        return wellformed_what.c_str();
      }

    private:
      std::string wellformed_what;
    };


    void usage() {
      try
      {
        throw CustomException(__LINE__, __FILE__);
      }
      catch (const CustomException& ex)
      {
        std::cout << ex.what() << std::endl;
      }
    }
  } // namespace custom

  namespace rethrow {
    std::exception_ptr eptr; // object to hold exceptions (or nullptr)
    void foo()
    {
      try {
        throw new std::runtime_error("asdasd");
      }
      catch (...) {
        eptr = std::current_exception(); // save exception for later processing
      }
    }

    void bar()
    {
      if (eptr != nullptr) {
        std::rethrow_exception(eptr); // process saved exception
      }
    }

    void usage() {
      foo();
      try
      {
        // lately... 
        bar();
      }
      catch (const std::exception& ex)
      {
        std::cout << ex.what() << std::endl;
      }
      catch (std::exception* ex)
      {
        std::cout << ex->what() << std::endl;
      }
      catch (...) {
        std::cout << "catched unknown exception!" << std::endl;
      }
    }
  } // namespace rethrow 

  namespace unwinding
  {
    struct unwinder
    {
      virtual int unwind() const = 0;
    };

    void will_throw()
    {
      int x = 7;
      struct unwinder_impl : public unwinder
      {
        int& val;                           // x ==> 8
        unwinder_impl(int& _val) : val(_val) { ++val; printf("%s\n", __FUNCTION__); }
                                            // x = 8
        unwinder_impl(const unwinder_impl& oth) : val(oth.val) { printf("%s\n", __FUNCTION__); }
        ~unwinder_impl() { printf("%s\n", __FUNCTION__); }

        int unwind() const override
        {
          return ++val; // x(val) = ?
        }
      } _unwinder{ x };
      throw _unwinder;
    }

    void usage()
    {
      try
      {
        will_throw();
      }
      catch (const unwinder& _unwinder)
      {
        int res = _unwinder.unwind();
        std::cout << "result: " << res << "\n";
      }
    }

  }
} // namespace exceptions

CREATE_ELEMENT_WITH_CODE(ExceptionUsage) {
  using namespace exceptions;
  ordering::usage();
  custom::usage();
  rethrow::usage();
  unwinding::usage();
}