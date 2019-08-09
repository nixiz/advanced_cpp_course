#pragma once
#include <playground_organizer.hpp>
#include <algorithm>
#include <numeric>

namespace packed_data_access
{
  struct AlignedStruct
  {
    char c;
    double value;
  };

#ifdef WIN32

#pragma pack(push,1)
  struct PackedStruct
  {
    char c;
    double value;
  };
#pragma pack(pop)

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
  // Visual Studio compilers are crashing due to 
  // their bugs when using nested constexpr structs
  // bu sebepten dolayi derleyici zamani yerine programin 
  // calistirildiginda static olarak yaratiyorum
  static StructCreator<AlignedStruct, 1024 *1024> list;
  static StructCreator<PackedStruct, 1024 * 1024> packed_list;

#else
//#include <cstdint>

  struct PackedStruct {
    char c;
    double value;
  } __attribute__((packed));

  template <typename S, int Size>
  struct StructCreator
  {
    constexpr StructCreator() : arr(), size(Size - 1)
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
#endif

}

// iki yapinin icerdigi veriler ayni olsa da uzunluklari farkli olacaktir
constexpr auto AlignedStructSize = sizeof(packed_data_access::AlignedStruct);
constexpr auto PackedStructSize = sizeof(packed_data_access::PackedStruct);
static_assert(AlignedStructSize != PackedStructSize, "packed struct size must be less than aligned one!");

auto sum_lambda = [] (int sum, const auto& s) {
  return sum + (s.c * s.value);
};

CREATE_ELEMENT_WITH_CODE(AlignedStructAccess) {
  using namespace packed_data_access;
#ifndef WIN32
  constexpr auto list = StructCreator<AlignedStruct, 1024*10>();
#endif
  int sum = std::accumulate(&list.arr[0], &list.arr[list.size], 0, sum_lambda);
  std::cout << "sum: " << sum << "\n";
}

CREATE_ELEMENT_WITH_CODE(PackedStructAccess) {
  using namespace packed_data_access;
#ifndef WIN32
  constexpr auto packed_list = StructCreator<PackedStruct, 1024*10>();
#endif
  int sum = std::accumulate(&packed_list.arr[0], &packed_list.arr[packed_list.size], 0, sum_lambda);
  std::cout << "sum: " << sum << "\n";
}
