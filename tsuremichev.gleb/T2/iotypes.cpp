#include "iotypes.hpp"
#include <cctype>

std::istream &operator>>(std::istream &in, DelimetrIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;

  char c = '0';
  in >> c;
  if (in && (c != dest.exp))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

// Парсинг комплексного числа формата #c(1.0 -1.0)
std::istream &operator>>(std::istream &in, ComplexLspIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;

  double real = 0.0;
  double imag = 0.0;

  in >> DelimetrIO{'#'} >> DelimetrIO{'c'} >> DelimetrIO{'('} >> real >> imag >> DelimetrIO{')'};

  if (in)
  {
    dest.ref = std::complex<double>(real, imag);
  }
  return in;
}

// Парсинг вещественного числа в научном формате (например, 5.45e-2)
std::istream &operator>>(std::istream &in, DoubleSciIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;

  double val = 0.0;
  // std::cin умеет автоматически распознавать экспоненциальный формат e/E через стандартный >> double
  if (in >> val)
  {
    dest.ref = val;
  }
  return in;
}

std::istream &operator>>(std::istream &in, StringIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;
  return std::getline(in >> DelimetrIO{'"'}, dest.ref, '"');
}

std::istream &operator>>(std::istream &in, KeyIO &&dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
    return in;

  while (in && std::isalnum(in.peek()))
  {
    dest.ref.push_back(in.get());
  }

  bool isKeyCorrect = dest.ref == "key1" || dest.ref == "key2" || dest.ref == "key3";
  if (!(in && isKeyCorrect))
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}
