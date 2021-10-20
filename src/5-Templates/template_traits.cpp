#include <advanced_cpp_topics.h>
#include <cstring>

namespace template_traits
{
  namespace problem
  {
    const unsigned max_chunk = 100;

    // works for ascii only
    class stream_buffer
    {
    public:
      // ...
      // return the next character or EOF
      int sgetc() {
        return EOF;
      }
    };

    // works when unicode
    class stream_buffer_unicode
    {
    public:
      // ...
      // return the next character or EOF
      wint_t sgetc() {
        return WEOF;
      }
    };


    void demonstrate()
    {
#if UNICODE
      stream_buffer_unicode buffer;
      // get buffer and process
      wint_t ch;
      while ((ch = buffer.sgetc()) != WEOF)
      {
        // process buffer
        putwchar(ch);
      }
#else
      stream_buffer buffer;
      // get buffer and process
      int ch;
      while ((ch = buffer.sgetc()) != EOF)
      {
        // process buffer
        putchar(ch);
      }
#endif
    }

  } // namespace problem

  namespace solution
  {
    template <typename CharT>
    struct char_traits { };
    // specialization for ascii chars
    template <>
    struct char_traits<char> {
      using char_type = char;
      using int_type = int;
      static inline int_type eof() { return EOF; }
    };
    // specialization for unicode chars
    template <>
    struct char_traits<wchar_t> {
      using char_type = wchar_t;
      using int_type = wint_t;
      static inline int_type eof() { return WEOF; }
    };

    // now we can support different char representations by
    // using trait helpers
    template <typename CharT>
    class basic_stream_buffer
    {
    public:
      using traits_type = char_traits<CharT>;
      using int_type = traits_type::int_type;
      // ...
      static int_type eof() { return traits_type::eof(); }
      // return the next character or EOF
      int_type sgetc() {
        return traits_type::eof();
      }
    };

#if UNICODE
    using basic_stream = basic_stream_buffer<wchar_t>;
#else
    using basic_stream = basic_stream_buffer<char>;
#endif
    void demonstrate()
    {
      basic_stream buffer;
      // get buffer and process
      basic_stream::int_type ch;
      while ((ch = buffer.sgetc()) != basic_stream::eof())
      {
        // process buffer
        putchar(ch);
      }
    }
  } // namespace solution

  namespace type_traits
  {

    template <typename Type>
    struct is_integer {
      static const bool val = false;
    };

    template <>
    struct is_integer<int> {
      static const bool val = true;
    };

    template <>
    struct is_integer<unsigned int> {
      static const bool val = true;
    };
/*
The simplest solution to compile-time assertions (Van Horn 1997), and one that works in C as well as in
C++, relies on the fact that a zero-length array is illegal.
*/
#define STATIC_CHECK(expr) { char unnamed[(expr) ? 1 : 0]; }

    template <typename T>
    void will_work_for_only_integer_types(T a, T b)
    {
      STATIC_CHECK(is_integer<T>::val);
    }

#include <type_traits>

    template <typename T, 
              typename = std::enable_if_t<std::is_integral<T>::value>>
    T will_work_for_only_integer_types_cpp11(T a, T b)
    {
      return a + b;
    }


  } // namespace type_traits
}

ELEMENT_CODE(TemplateTraits)
{

}
