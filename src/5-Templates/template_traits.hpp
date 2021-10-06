#pragma once
#include <playground_organizer.hpp>
#include <string>

namespace template_traits
{
  namespace problem
  {
    const unsigned max_chunk = 100;
    // 
    class stream_buffer
    {
    public:
      stream_buffer(const std::string& input) {
        buff = new char[input.size()];
        memcpy(buff, input.c_str(), input.size());
      }

      ~stream_buffer()
      {
        delete buff;
      }

      // ...
      int sgetc(); // return the next character or EOF
      int sgetn(char *, int N); // get N characters from buffer
    private:
      char *buff;
    };

  } // namespace problem
}

CREATE_ELEMENT_WITH_CODE(TemplateTraits) {

}
