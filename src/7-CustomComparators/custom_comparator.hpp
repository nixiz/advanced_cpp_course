#pragma once
#include <playground_organizer.hpp>
#include <iostream>
#include <chrono>
#include <utility>
#include <cmath>
#include <limits>
#include <functional>
#include <map>
#include <set>
#include <type_traits>

class ICTerraValueComparator {
private:
  bool bUseCompEpsl;
  double dEpsl;
public:
  explicit ICTerraValueComparator(double epsl = 0) : dEpsl(epsl) {
    bUseCompEpsl = dEpsl == 0 ? false : true;
  }
  template <class T,
    typename = typename std::enable_if<std::is_floating_point<T>::value>::type >
  bool operator()(const T& lhs, const T& rhs) const {
    bool b = std::fabs(lhs - rhs) > (bUseCompEpsl ? static_cast<T>(dEpsl) : std::numeric_limits<T>::epsilon()) && 
             (lhs < rhs);
    return b;
  }

  // all other types
  bool operator()(const int& lhs, const int& rhs) const {
    return lhs < rhs;
  }

};

namespace ccn {

  class Student {
  private:
    int average;
    std::string _firstname;
    std::string _lastname;
  public:
    Student() = delete;
    Student(std::string firstname, std::string lastname,  int aver) : 
      _firstname(firstname), _lastname(lastname), average(aver) { }
    int getAverage() const { return average; }
    std::string getName() const {
      std::ostringstream o_str;
      o_str
        << _firstname << " " << _lastname;
      return o_str.str();
    }
  };

  // Sadece Student tipteki iterator tipleri için geçerli olacaktýr.
  // Bu sayede, fonksiyon içerisinde begin -> end arasýndaki iterasyonlarda 
  // Student sýnýfýnýn özelliklerine eriþebileceðimiz garanti altýna almýþ oluyoruz.
  template <typename Iter, 
            typename = typename std::enable_if<
                          std::is_same<typename std::iterator_traits<Iter>::value_type, Student*>::value 
                       >::type >
  void printStudents(Iter begin, Iter end) {
    for (Iter it = begin; it != end; it++)
    {
      std::cout << "Student: " << (*it)->getName() << std::endl;
    }
  }

  void problem() {
    std::set<Student*> mset;
    mset.insert(new Student("Lakesha", "Fleming", 10));
    mset.insert(new Student("Alexandria", "Najar", 30));
    mset.insert(new Student("Elliot", "Riss", 20));
    mset.insert(new Student("Aleida", "Hamblen", 50));
    printStudents(mset.begin(), mset.end());
  }

  namespace solution {

    struct AddStudentAverageAscending {
      bool operator()(Student* lhs, Student* rhs) {
        return lhs->getAverage() < rhs->getAverage();
      }
    };

    struct AddStudentAverageDescending {
      bool operator()(Student* lhs, Student* rhs) {
        return lhs->getAverage() > rhs->getAverage();
      }
    };

    void usage() {
      std::set<Student*, AddStudentAverageDescending> mset;
      mset.insert(new Student("Lakesha", "Fleming", 10));
      mset.insert(new Student("Alexandria", "Najar", 30));
      mset.insert(new Student("Elliot", "Riss", 20));
      mset.insert(new Student("Aleida", "Hamblen", 50));
      printStudents(mset.begin(), mset.end());
    }

  } // namespace problem 

} // namespace ccn



CREATE_ELEMENT_WITH_CODE(CustomComparatorNeedExample) {

  ccn::problem();
  ccn::solution::usage();
}

CREATE_ELEMENT_WITH_CODE(CustomComparatorExample) {

  {
    std::map<float, int, ICTerraValueComparator> mm; // use numeric limit epsilon
    mm[1.0] = 1;
    mm[2.0] = 2;
    mm[3.0] = 3;
    auto f = mm.find(1.0);
  }

  {
    std::map<float, int, ICTerraValueComparator> mm(ICTerraValueComparator(1.1));
    mm[1.0] = 1;
    mm[2.0] = 2;
    mm[3.0] = 3;
    auto f = mm.find(1.0);
  }

  {
    std::map<int, int, ICTerraValueComparator> mm(ICTerraValueComparator(0));
    mm[1] = 1;
    mm[2] = 2;
    mm[3] = 3;
    auto f = mm.find(1);
  }
}
