#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include <iostream>
#include <string>
#include <complex>
#include <utility>

struct DataStruct {
    enum Type { NONE, DOUBLE, LONG_LONG, UNSIGNED_LONG_LONG, CHAR, COMPLEX, RATIONAL };
    Type type1, type2;
    double double1, double2;
    long long ll1, ll2;
    unsigned long long ull1, ull2;
    char char1, char2;
    std::complex<double> complex1, complex2;
    std::pair<long long, unsigned long long> rational1, rational2;
    std::string key3;
    DataStruct();
};

std::istream& operator>>(std::istream& in, DataStruct& data);
std::ostream& operator<<(std::ostream& out, const DataStruct& data);
bool comparator(const DataStruct& a, const DataStruct& b);

#endif
