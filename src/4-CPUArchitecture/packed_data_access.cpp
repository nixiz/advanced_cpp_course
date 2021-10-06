#include <advanced_cpp_topics.h>
#include <iostream>
#include <algorithm>
#include <numeric>

namespace packed_data_access
{
  struct AlignedStruct
  {
    char c;
    double value;
  };

#ifdef _WIN32
#pragma pack(push,1)
  struct PackedStruct
  {
    char c;
    double value;
  };
#pragma pack(pop)
#else
  struct PackedStruct {
    char c;
    double value;
  } __attribute__((packed));
#endif

  template <typename S, int Size>
  struct StructCreator
  {
    StructCreator() : size(Size - 1)
    {
      auto len = size;
      while (--len > 0)
      {
        arr[len].c = 'a' + (len % 20);
        arr[len].value = len / 1000.0;
      }
    }
    std::size_t size;
    S arr[Size];
  };
  static StructCreator<AlignedStruct, 1024 * 1024> aligned_list;
  static StructCreator<PackedStruct, 1024 * 1024> packed_list;
}

// iki yapinin icerdigi veriler ayni olsa da uzunluklari farkli olacaktir
constexpr auto AlignedStructSize = sizeof(packed_data_access::AlignedStruct);
constexpr auto PackedStructSize = sizeof(packed_data_access::PackedStruct);
static_assert(AlignedStructSize != PackedStructSize, "packed struct size must be less than aligned one!");

auto sum_lambda = [](int sum, const auto& s) {
  return sum + (unsigned int)(s.c * s.value);
};

ELEMENT_CODE(AlignedStructAccess) {
  using namespace packed_data_access;
  unsigned int sum = std::accumulate(&aligned_list.arr[0], &aligned_list.arr[aligned_list.size], 0, sum_lambda);
  std::cout << "sum: " << sum << "\n";
}

ELEMENT_CODE(PackedStructAccess) {
  using namespace packed_data_access;
  unsigned int sum = std::accumulate(&packed_list.arr[0], &packed_list.arr[packed_list.size], 0, sum_lambda);
  std::cout << "sum: " << sum << "\n";
}
