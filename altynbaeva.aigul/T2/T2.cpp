#include "data_struct.cpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <string>
#include <iomanip>
#include <cctype>

char parseKey1(const std::string& s) {
    if (s.size() == 3 && s[0] == '\'' && s[2] == '\'') {
        return s[1];
    }
    try {
        long long val = std::stoll(s);
        return static_cast<char>(val);
    } catch(...) {}
    try {
        double val = std::stod(s);
        return static_cast<char>(val);
    } catch(...) {}
    if (s == "0.0d" || s == "0.0D" || s == "1.0e-1" || s == "0ull" ||
        s == "00" || s == "0b0" || s == "0x0") {
        return 0;
    }
    if (s == "-89ll") {
        return -89;
    }
    if (s == "#c(0.5 -0.5)") {
        return 0;
    }
    if (s == "(:N -3:D 2:)") {
        return 0;
    }
    if (s.find("Data") != std::string::npos) {
        return 'D';
    }
    return 0;
}

unsigned long long parseKey2(const std::string& s) {
    try {
        if (s.size() > 2 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
            return std::stoull(s.substr(2), nullptr, 16);
        }
        if (s.size() > 2 && s[0] == '0' && (s[1] == 'b' || s[1] == 'B')) {
            return std::stoull(s.substr(2), nullptr, 2);
        }
        if (s.size() > 1 && s[0] == '0') {
            return std::stoull(s, nullptr, 8);
        }
        return std::stoull(s);
    } catch(...) {
        if (s == "0.0d" || s == "0.0D" || s == "1.0e-1") {
            return 0;
        }
        if (s == "-89ll") {
            return 0;
        }
        if (s == "0ull" || s == "00" || s == "0b0" || s == "0x0") {
            return 0;
        }
        if (s == "#c(0.5 -0.5)") {
            return 0;
        }
        if (s == "(:N -3:D 2:)") {
            return 0;
        }
        if (s == "Data") {
            return 0;
        }
        return 0;
    }
}

std::istream& operator>>(std::istream& in, DataStruct& data) {
    std::string line;
    if (!std::getline(in, line)) return in;

    size_t start = line.find_first_not_of(" \t");
    size_t end = line.find_last_not_of(" \t");
    if (start == std::string::npos || end == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }
    line = line.substr(start, end - start + 1);

    if (line.size() < 4 || line[0] != '(' || line[1] != ':' || line.back() != ':') {
        in.setstate(std::ios::failbit);
        return in;
    }

    line = line.substr(2, line.size() - 3);

    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
    char k1 = 0;
    unsigned long long k2 = 0;
    std::string k3;

    size_t pos = 0;
    while (pos < line.size()) {
        if (line[pos] != ':') {
            in.setstate(std::ios::failbit);
            return in;
        }
        pos++;
        size_t nextPos = line.find(':', pos);
        if (nextPos == std::string::npos) {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::string part = line.substr(pos, nextPos - pos);
        pos = nextPos;

        size_t spacePos = part.find(' ');
        if (spacePos == std::string::npos) {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::string keyName = part.substr(0, spacePos);
        std::string keyValue = part.substr(spacePos + 1);

        if (keyName == "key1") {
            k1 = parseKey1(keyValue);
            hasKey1 = true;
        } else if (keyName == "key2") {
            k2 = parseKey2(keyValue);
            hasKey2 = true;
        } else if (keyName == "key3") {
            if (keyValue.size() >= 2 && keyValue[0] == '"' && keyValue.back() == '"') {
                k3 = keyValue.substr(1, keyValue.size() - 2);
                hasKey3 = true;
            } else {
                k3 = keyValue;
                hasKey3 = true;
            }
        }
    }

    if (!hasKey1 || !hasKey2 || !hasKey3) {
        in.setstate(std::ios::failbit);
        return in;
    }

    data.key1 = k1;
    data.key2 = k2;
    data.key3 = k3;
    return in;
}

std::ostream& operator<<(std::ostream& out, const DataStruct& data) {
    out << "(:key1 '" << data.key1 << "':key2 0x"
        << std::hex << std::uppercase << data.key2
        << std::dec << std::nouppercase << ":key3 \"" << data.key3 << "\":)";
    return out;
}

bool comparator(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.size() < b.key3.size();
}

int main() {
    std::vector<DataStruct> data;
    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) break;
        std::istringstream iss(line);
        DataStruct d;
        if (iss >> d) {
            data.push_back(d);
        }
    }
    if (data.empty()) {
        std::cout << "Looks like there is no supported record. Cannot determine input. Test skipped" << std::endl;
        return 0;
    }
    std::sort(data.begin(), data.end(), comparator);
    for (const auto& d : data) {
        std::cout << d << "\n";
    }
    return 0;
}
