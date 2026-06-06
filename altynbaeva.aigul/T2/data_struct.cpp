#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <stdexcept>

struct DataStruct {
    char key1;
    unsigned long long key2;
    std::string key3;
};

char parseKey1(const std::string& s) {
    // Только формат 'X' (CHR LIT)
    if (s.size() == 3 && s[0] == '\'' && s[2] == '\'') {
        return s[1];
    }
    throw std::invalid_argument("Invalid char literal");
}

unsigned long long parseKey2(const std::string& s) {
    if (s.empty()) throw std::invalid_argument("Empty string");

    // ULL LIT: с суффиксом ull/ULL
    if (s.size() >= 3) {
        std::string suffix = s.substr(s.size() - 3);
        if (suffix == "ull" || suffix == "ULL") {
            std::string num = s.substr(0, s.size() - 3);
            return std::stoull(num, nullptr, 10);
        }
    }

    // ULL HEX: 0x...
    if (s.size() >= 3 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
        return std::stoull(s.substr(2), nullptr, 16);
    }

    // ULL BIN: 0b...
    if (s.size() >= 3 && s[0] == '0' && (s[1] == 'b' || s[1] == 'B')) {
        return std::stoull(s.substr(2), nullptr, 2);
    }

    // ULL OCT: 0...
    if (s.size() > 1 && s[0] == '0') {
        for (size_t i = 1; i < s.size(); ++i) {
            if (s[i] < '0' || s[i] > '7') {
                throw std::invalid_argument("Invalid octal");
            }
        }
        return std::stoull(s, nullptr, 8);
    }

    throw std::invalid_argument("Invalid ULL format");
}

std::istream& operator>>(std::istream& in, DataStruct& data) {
    std::string line;
    if (!std::getline(in, line)) {
        in.setstate(std::ios::failbit);
        return in;
    }

    // Trim
    size_t start = line.find_first_not_of(" \t\r\n");
    size_t end = line.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) {
        in.setstate(std::ios::failbit);
        return in;
    }
    line = line.substr(start, end - start + 1);

    // Проверяем формат (: ... :)
    if (line.size() < 4 || line.substr(0, 2) != "(:" || line.substr(line.size() - 2) != ":)") {
        in.setstate(std::ios::failbit);
        return in;
    }

    // Убираем (: и :)
    line = line.substr(2, line.size() - 4);

    // Разделяем по ':'
    std::vector<std::string> parts;
    size_t pos = 0;
    size_t nextPos = line.find(':');
    while (nextPos != std::string::npos) {
        parts.push_back(line.substr(pos, nextPos - pos));
        pos = nextPos + 1;
        nextPos = line.find(':', pos);
    }
    parts.push_back(line.substr(pos));

    if (parts.size() != 3) {
        in.setstate(std::ios::failbit);
        return in;
    }

    bool hasKey1 = false, hasKey2 = false, hasKey3 = false;
    char k1 = 0;
    unsigned long long k2 = 0;
    std::string k3;

    for (const auto& part : parts) {
        size_t spacePos = part.find(' ');
        if (spacePos == std::string::npos) {
            in.setstate(std::ios::failbit);
            return in;
        }
        std::string keyName = part.substr(0, spacePos);
        std::string keyValue = part.substr(spacePos + 1);

        try {
            if (keyName == "key1") {
                k1 = parseKey1(keyValue);
                hasKey1 = true;
            } else if (keyName == "key2") {
                k2 = parseKey2(keyValue);
                hasKey2 = true;
            } else if (keyName == "key3") {
                if (keyValue.size() >= 2 && keyValue[0] == '"' && keyValue.back() == '"') {
                    k3 = keyValue.substr(1, keyValue.size() - 2);
                } else {
                    k3 = keyValue;
                }
                hasKey3 = true;
            }
        } catch (...) {
            in.setstate(std::ios::failbit);
            return in;
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
        << std::dec << std::nouppercase
        << ":key3 \"" << data.key3 << "\":)";
    return out;
}

bool comparator(const DataStruct& a, const DataStruct& b) {
    if (a.key1 != b.key1) return a.key1 < b.key1;
    if (a.key2 != b.key2) return a.key2 < b.key2;
    return a.key3.size() < b.key3.size();
}
