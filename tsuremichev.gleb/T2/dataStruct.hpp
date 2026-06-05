#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iostream>
#include <string>
#include <complex>
#include "iotypes.hpp"
#include "iofmtguard.hpp"

struct DataStruct
{
  std::complex<double> key1{0.0, 0.0};
  double key2 = 0.0;
  std::string key3;
};

std::istream &operator>>(std::istream &in, DataStruct &dest);
std::ostream &operator<<(std::ostream &out, const DataStruct &src);
bool operator<(const DataStruct &lhs, const DataStruct &rhs);

#endif
