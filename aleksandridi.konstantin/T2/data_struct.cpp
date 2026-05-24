#include "data_struct.hpp"
#include "iofmtguard.hpp"
#include <iostream>
#include <cctype>
#include <iomanip>
#include <cmath>

namespace nspace {
    std::istream &operator>>(std::istream &in, DelimiterIO &&dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }
        char c = 0;
        in >> c;
        if (in && c != dest.expected) {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::istream &operator>>(std::istream &in, DoubleSciIO &&dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        std::string buffer;
        char c = 0;
        bool has_dot = false;
        bool has_exp = false;

        while (in >> c) {
            if (c == '.') {
                has_dot = true;
            } else if (c == 'e' || c == 'E') {
                has_exp = true;
            } else if (c == ':') {
                in.putback(':');
                break;
            }
            buffer += c;
        }

        if (!has_dot || !has_exp) {
            in.setstate(std::ios::failbit);
            return in;
        }

        try {
            std::size_t processed = 0;
            dest.ref = std::stod(buffer, &processed);
            if (processed != buffer.size()) {
                in.setstate(std::ios::failbit);
            }
        } catch (...) {
            in.setstate(std::ios::failbit);
        }

        return in;
    }

    std::istream &operator>>(std::istream &in, CharLitIO &&dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }
        char open_quote = 0, value = 0, close_quote = 0;
        in >> open_quote >> value >> close_quote;
        if (in && (open_quote != '\'' || close_quote != '\'')) {
            in.setstate(std::ios::failbit);
        } else {
            dest.ref = value;
        }
        return in;
    }

    std::istream &operator>>(std::istream &in, StringIO &&dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }
        return std::getline(in >> DelimiterIO{'"'}, dest.ref, '"');
    }

    std::istream &operator>>(std::istream &in, DataStruct &dest) {
        std::istream::sentry sentry(in);
        if (!sentry) {
            return in;
        }

        DataStruct temp;
        bool parsed_k1 = false, parsed_k2 = false, parsed_k3 = false;

        in >> DelimiterIO{'('};

        for (int i = 0; i < 3; ++i) {
            in >> DelimiterIO{':'};

            char k = 0, e = 0, y = 0, num = 0;
            in >> k >> e >> y >> num;

            if (!in || k != 'k' || e != 'e' || y != 'y') {
                in.setstate(std::ios::failbit);
                return in;
            }

            if (num == '1' && !parsed_k1) {
                in >> DoubleSciIO{temp.key1};
                parsed_k1 = static_cast<bool>(in);
            } else if (num == '2' && !parsed_k2) {
                in >> CharLitIO{temp.key2};
                parsed_k2 = static_cast<bool>(in);
            } else if (num == '3' && !parsed_k3) {
                in >> StringIO{temp.key3};
                parsed_k3 = static_cast<bool>(in);
            } else {
                in.setstate(std::ios::failbit);
                return in;
            }
        }

        in >> DelimiterIO{':'} >> DelimiterIO{')'};

        if (in && parsed_k1 && parsed_k2 && parsed_k3) {
            dest = temp;
        } else {
            in.setstate(std::ios::failbit);
        }
        return in;
    }

    std::ostream &operator<<(std::ostream &out, const DataStruct &src) {
        std::ostream::sentry sentry(out);
        if (!sentry) {
            return out;
        }
        iofmtguard guard(out);

        out << "(:";
        out << "key1 " << std::scientific << std::setprecision(1) << src.key1 << ':';
        out << "key2 '" << src.key2 << "':";
        out << "key3 \"" << src.key3 << "\":)";
        return out;
    }

    bool compareDataStruct(const DataStruct &lhs, const DataStruct &rhs) {
        if (lhs.key1 != rhs.key1) {
            return lhs.key1 < rhs.key1;
        }
        if (lhs.key2 != rhs.key2) {
            return lhs.key2 < rhs.key2;
        }
        return lhs.key3.size() < rhs.key3.size();
    }
}
