#include "Polygon.h"
#include <algorithm>
#include <string>

bool operator==(const Point& a, const Point& b) {
    return a.x == b.x && a.y == b.y;
}

bool operator==(const Polygon& a, const Polygon& b) {
    if (a.points.size() != b.points.size()) return false;
    return std::equal(a.points.begin(), a.points.end(), b.points.begin());
}

std::istream& operator>>(std::istream& in, Point& p) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    char c1, c2, c3;
    int x, y;
    if (!(in >> c1) || c1 != '(') { in.setstate(std::ios::failbit); return in; }
    if (!(in >> x)) return in;
    if (!(in >> c2) || c2 != ';') { in.setstate(std::ios::failbit); return in; }
    if (!(in >> y)) return in;
    if (!(in >> c3) || c3 != ')') { in.setstate(std::ios::failbit); return in; }
    p.x = x; p.y = y;
    return in;
}

std::istream& operator>>(std::istream& in, Polygon& p) {
    std::istream::sentry sentry(in);
    if (!sentry) return in;
    size_t count;
    if (!(in >> count) || count < 3) { in.setstate(std::ios::failbit); return in; }
    std::vector<Point> temp;
    for (size_t i = 0; i < count; ++i) {
        Point pt;
        if (!(in >> pt)) { in.setstate(std::ios::failbit); return in; }
        temp.push_back(pt);
    }
    p.points = std::move(temp);
    return in;
}
