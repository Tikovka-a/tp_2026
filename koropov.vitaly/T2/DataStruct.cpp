#include "DataStruct.hpp"
#include <iomanip>
#include <regex>
#include <sstream>

bool compareDataStruct(const DataStruct &a, const DataStruct &b)
{
  if (a.key1 != b.key1)
  {
    return a.key1 < b.key1;
  }
  if (a.key2 != b.key2)
  {
    return a.key2 < b.key2;
  }
  return a.key3.length() < b.key3.length();
}

std::istream &operator>>(std::istream &in, DataStruct &dest)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string line;
  if (!std::getline(in, line))
  {
    return in;
  }

  std::string dbl_lit_regex = R"(-?\d+\.\d+[dD])";
  std::string ull_hex_regex = R"(0[xX][0-9a-fA-F]+)";
  std::string str_regex = R"("[^"\\]*")";

  std::string field_regex = "key(1|2|3) (" + dbl_lit_regex + "|" + ull_hex_regex + "|" + str_regex + ")";

  std::regex full_pattern("^\\(:" + field_regex + ":" + field_regex + ":" + field_regex + ":\\)\\r?$");

  std::smatch match;
  if (!std::regex_match(line, match, full_pattern))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  std::regex key_value_pair_regex("key(1|2|3) ([^:]+)");
  auto words_begin = std::sregex_iterator(line.begin(), line.end(), key_value_pair_regex);
  auto words_end = std::sregex_iterator();

  bool has_key1 = false, has_key2 = false, has_key3 = false;
  double t_key1 = 0.0;
  unsigned long long t_key2 = 0;
  std::string t_key3 = "";

  for (std::sregex_iterator i = words_begin; i != words_end; ++i)
  {
    std::smatch m = *i;
    int key_num = std::stoi(m[1].str());
    std::string val_str = m[2].str();

    if (key_num == 1)
    {
      val_str.pop_back();
      std::stringstream ss(val_str);
      ss >> t_key1;
      has_key1 = true;
    }
    else if (key_num == 2)
    {
      t_key2 = std::stoull(val_str, nullptr, 16);
      has_key2 = true;
    }
    else if (key_num == 3)
    {
      t_key3 = val_str.substr(1, val_str.length() - 2);
      has_key3 = true;
    }
  }

  if (has_key1 && has_key2 && has_key3)
  {
    dest.key1 = t_key1;
    dest.key2 = t_key2;
    dest.key3 = t_key3;
  }
  else
  {
    in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream &operator<<(std::ostream &out, const DataStruct &src)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  std::ios_base::fmtflags flags = out.flags();

  out << "(:key1 " << std::fixed << std::setprecision(1) << src.key1 << "d"
      << ":key2 0x" << std::hex << std::uppercase << src.key2
      << ":key3 \"" << src.key3 << "\":)";

  out.flags(flags);
  return out;
}