#ifndef OP_OVERLOAD_EXAMPLE_H_
#define OP_OVERLOAD_EXAMPLE_H_
#include <playground_organizer.hpp>
#include "src/0-Types/number.hpp"

using namespace std;

CREATE_ELEMENT_WITH_CODE(OperatorOverloadOverrideExample)
{
  Number num;
  num = num + 10; // calls {1}
  num += 10;      // calls {2}
  num++;          // calls {3}
  ++num;          // calls {4}
  std::cout << num.toString() << std::endl;

  ComplexNumber cnum;
  cnum.set(5, 1);
  cnum++;
  std::cout << cnum.toString() << std::endl;
}
#endif