#ifndef _NUMBER_HPP_
#define _NUMBER_HPP_

#include <iostream>
#include <sstream>

struct Number {
  typedef int value_type;

  explicit Number(int initial = int()) : n(initial) { printf("\n%s", __FUNCTION__); }
  ~Number() { printf("\n%s", __FUNCTION__); }
  void set(int v) { n = v; }
  int get() const { return n; }

  Number& operator+(const int& val) // {1}
  {
    this->n += val;
    return *this;
  }

  Number& operator+=(const int& val) // {2}
  {
    this->n += val;
    return *this;
  }

  // {3} ve {4} birlikte olmazsa build olmaz. iki tip prefix (a++ ve ++a) için de op overload olmasý gerekiyor
  Number& operator++() // {3}
  {
    this->n += 1;
    return *this;
  }

  Number& operator++(int) // {4}
  {
    this->n += 1;
    return *this;
  }

  std::string toString() const
  {
    std::ostringstream o_str;
    o_str << n;
    return o_str.str();
  }
private:
  int n;
};

struct ComplexNumber : public Number {

  void set(int v, int complex) {
    Number::set(v);
    _complex = complex;
  }

  int getComplex() const { return _complex; }

  // overrided
  std::string toString() const
  {
    std::ostringstream o_str;
    o_str << "real: " << get() << " complex: " << getComplex();
    return o_str.str();
  }

  ComplexNumber& operator+(const ComplexNumber& val) // {1}
  {
    set(val.get(), val.getComplex());
    return *this;
  }

  ComplexNumber& operator+=(const ComplexNumber& val) // {2}
  {
    set(val.get(), val.getComplex());
    return *this;
  }

  ComplexNumber& operator++() // {3}
  {
    set(get() + 1, getComplex());
    return *this;
  }

  ComplexNumber& operator++(int) // {4}
  {
    set(get() + 1, getComplex());
    return *this;
  }
private:
  int _complex;
};


#endif