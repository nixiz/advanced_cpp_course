#pragma once
#include <playground_organizer.hpp>
#include "src/0-Types/number.hpp"

namespace mallocusage {

  void DoublePointerUsage() {
    // create 3x3 matrix using malloc
    int** mat = (int **)malloc(3 * sizeof(int*));
    for (int i = 0; i<3; ++i) {
      mat[i] = (int *)malloc(3 * sizeof(int));
    }
    // empty matrix
    for (size_t i = 0; i < 3; i++)
    {
      for (size_t j = 0; j < 3; j++)
      {
        mat[i][j] = 0;
      }
    }

    // free the matrix pointer with reverse order of allocation
    for (int i = 0; i<3; ++i) {
      free(mat[i]);
    }
    free(mat);
  }

  void CallocUsage() {
    // create 3x3 matrix using malloc
    int** mat = (int **)calloc(3, sizeof(int*));
    // calloc enables to reallocate int ** size as needed
    for (int i = 0; i<3; ++i) {
      mat[i] = (int *)calloc(3, sizeof(int));
    }
    // created by calloc so all values should be 0 by default
    for (size_t i = 0; i < 3; i++)
    {
      for (size_t j = 0; j < 3; j++)
      {
        printf("%d", mat[i][j]);
      }
    }

    // free the matrix pointer with reverse order of allocation
    for (int i = 0; i<3; ++i) {
      free(mat[i]);
    }
    free(mat);
  }

  void MallocWithClassesBad() {
    Number* num = (Number*)malloc(sizeof(Number));
    if (num == NULL) _ASSERT_EXPR(false, "cannot allocate memory for Number obj");
    std::cout << "Current number: " << num->get() << std::endl;
    free(num);
  }

  void MallocWithClassesGood() {
    Number* num = (Number*)malloc(sizeof(Number));
    new(num) Number(); // call constructor of allocated instance
    std::cout << "Current number: " << num->get() << std::endl;
    free(num);
  }

  void MallocWithClassesInStack() {
    // create buffer for Number class in stack
    alignas(Number) char localbuffer[sizeof(Number)]{ 0 };
    Number* num = new(localbuffer) Number(); // call constructor of allocated instance
    num->set(10);
  }
} // namespace mallocusage

CREATE_ELEMENT_WITH_CODE(MallocUsageExample) {
  mallocusage::DoublePointerUsage();
  mallocusage::CallocUsage();
  {
    mallocusage::MallocWithClassesInStack();
  }
  mallocusage::MallocWithClassesBad();
  mallocusage::MallocWithClassesGood();
}
