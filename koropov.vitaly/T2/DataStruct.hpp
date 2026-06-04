#ifndef DATA_STRUCT_HPP
#define DATA_STRUCT_HPP

#include <iostream>
#include <string>
#include <vector>

struct DataStruct
{
  double key1;
  unsigned long long key2;
  std::string key3;
};

bool compareDataStruct(const DataStruct &a, const DataStruct &b);

std::istream &operator>>(std::istream &in, DataStruct &dest);
std::ostream &operator<<(std::ostream &out, const DataStruct &src);

#endif // DATA_STRUCT_HPP