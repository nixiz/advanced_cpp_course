#include <advanced_cpp_topics.h>
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
          << "Custom Exception: "
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
      catch (const std::exception& ex)
      {
        std::cout << ex.what() << std::endl;
      }
    }
  } // namespace custom

  namespace rethrow {
    std::exception_ptr eptr;
    void thread_func() try
    {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      throw std::runtime_error("abort thread");
    }
    catch (...) {
      eptr = std::current_exception(); // save exception for later processing
    }

    void usage() 
    {
      std::thread(thread_func).join();
      try {
        if (eptr != nullptr) {
          // process saved exception
          std::rethrow_exception(eptr);
        }
      } catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
      } catch (...) {
        std::cout << "catched unknown exception!" << std::endl;
      }
    }
  } // namespace rethrow 

  namespace unwinding
  {
    struct unwinder
    {
      virtual ~unwinder() = default;
      virtual void unwind() const = 0;
    };

    void will_throw(int should_throw)
    {
      int* val = new int(10);
      struct stack_cleaner final
        : public unwinder 
      {
        int*& val;
        stack_cleaner(int*& _val) 
          : val(_val) { printf("%s\n", __FUNCTION__); }
        ~stack_cleaner() { printf("%s\n", __FUNCTION__); }

        void unwind() const override {
          if (val) delete val;
        }
      };
      if (should_throw % 2 == 0)
        throw stack_cleaner(val);
      else
        delete val;
    }

    void unwinding_usage()
    {
      try
      {
        while (true)
        {
          will_throw(rand());
        }
      }
      catch (const unwinder& unwinder)
      {
        unwinder.unwind();
      }
    }

  }
} // namespace exceptions

ELEMENT_CODE(ExceptionUsage) {
  using namespace exceptions;
  ordering::usage();
  custom::usage();
  rethrow::usage();
}

ELEMENT_CODE(ExceptionUnwinding) {
  using namespace exceptions;
  unwinding::unwinding_usage();
}
