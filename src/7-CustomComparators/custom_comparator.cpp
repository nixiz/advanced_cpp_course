#include <advanced_cpp_topics.h>
#include <iostream>
#include <sstream>
#include <chrono>
#include <utility>
#include <cmath>
#include <limits>
#include <functional>
#include <map>
#include <set>
#include <type_traits>

class STMValueComparator {
private:
  bool bUseCompEpsl;
  double dEpsl;
public:
  explicit STMValueComparator(double epsl = 0) : dEpsl(epsl) {
    bUseCompEpsl = dEpsl == 0 ? false : true;
  }
  template <class T,
    typename = typename std::enable_if_t<std::is_floating_point<T>::value>>
    bool operator()(const T& lhs, const T& rhs) const {
    if (bUseCompEpsl)
      return (std::fabs(lhs - rhs) > static_cast<T>(dEpsl)) &&
      (lhs < rhs);
    else
      return (std::fabs(lhs - rhs) > std::numeric_limits<T>::epsilon()) &&
      (lhs < rhs);
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
    Student(std::string firstname, std::string lastname, int aver) :
      _firstname(firstname), _lastname(lastname), average(aver) { }
    int getAverage() const { return average; }
    std::string getName() const {
      std::ostringstream o_str;
      o_str
        << _firstname << " " << _lastname;
      return o_str.str();
    }
  };

  // Sadece Student tipteki iterator tipleri i�in ge�erli olacakt�r.
  // Bu sayede, fonksiyon i�erisinde begin -> end aras�ndaki iterasyonlarda 
  // Student s�n�f�n�n �zelliklerine eri�ebilece�imiz garanti alt�na alm�� oluyoruz.
  template <typename Iter,
    typename = typename std::enable_if<
    std::is_same<typename std::iterator_traits<Iter>::value_type, Student*>::value>::type >
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
      bool operator()(const Student* lhs, const Student* rhs) const {
        return lhs->getAverage() < rhs->getAverage();
      }
    };

    struct AddStudentAverageDescending {
      bool operator()(const Student* lhs, const Student* rhs) const {
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



ELEMENT_CODE(CustomComparatorNeedExample) {

  ccn::problem();
  ccn::solution::usage();
}

ELEMENT_CODE(CustomComparatorExample) {

  {
    std::map<float, int, STMValueComparator> mm; // use numeric limit epsilon
    mm[1.0] = 1;
    mm[2.0] = 2;
    mm[3.0] = 3;
    auto f = mm.find(1.0);
  }

  {
    std::map<float, int, STMValueComparator> mm(STMValueComparator(1.1));
    mm[1.0] = 1;
    mm[2.0] = 2;
    mm[3.0] = 3;
    auto f = mm.find(1.0);
  }

  {
    std::map<int, int, STMValueComparator> mm(STMValueComparator(0));
    mm[1] = 1;
    mm[2] = 2;
    mm[3] = 3;
    auto f = mm.find(1);
  }
}
