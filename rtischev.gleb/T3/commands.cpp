#include "commands.hpp"
#include "geometry.hpp"
#include "iofmtguard.hpp"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>
#include <string>

namespace Commands
{

    // AREA
    void cmd_AREA(const std::vector<Polygon> &polygons, std::istream &in, std::ostream &out)
    {
        std::string sub_cmd;
        if (!(in >> sub_cmd))
        {
            throw std::runtime_error("Invalid argument for AREA");
        }

        iofmtguard guard(out);
        out << std::fixed << std::setprecision(1);

        if (sub_cmd == "MEAN")
        {
            if (polygons.empty())
            {
                out << 0.0 << '\n';
                return;
            }
            double total_area = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                                                [](double acc, const Polygon &p)
                                                { return acc + Geometry::get_area(p); });
            out << (total_area / polygons.size()) << '\n';
        }
        else if (sub_cmd == "EVEN" || sub_cmd == "ODD")
        {
            size_t index = 0;
            bool target_even = (sub_cmd == "EVEN");

            double sum = std::accumulate(polygons.cbegin(), polygons.cend(), 0.0,
                                         [&index, target_even](double acc, const Polygon &p)
                                         {
                                             bool is_target_index = ((index % 2 == 0) == target_even);
                                             index++;
                                             return acc + (is_target_index ? Geometry::get_area(p) : 0.0);
                                         });
            out << sum << '\n';
        }
        else
        {
            throw std::runtime_error("Unknown sub-command for AREA");
        }
    }

    // MAX
    void cmd_MAX(const std::vector<Polygon> &polygons, std::istream &in, std::ostream &out)
    {
        if (polygons.empty())
        {
            throw std::runtime_error("Collection is empty");
        }

        std::string sub_cmd;
        if (!(in >> sub_cmd))
        {
            throw std::runtime_error("Invalid argument for MAX");
        }

        if (sub_cmd == "AREA")
        {
            auto it = std::max_element(polygons.begin(), polygons.end(),
                                       [](const Polygon &a, const Polygon &b)
                                       { return Geometry::get_area(a) < Geometry::get_area(b); });

            iofmtguard guard(out);
            out << std::fixed << std::setprecision(1) << Geometry::get_area(*it) << '\n';
        }
        else if (sub_cmd == "VERTICES")
        {
            auto it = std::max_element(polygons.begin(), polygons.end(),
                                       [](const Polygon &a, const Polygon &b)
                                       { return a.points.size() < b.points.size(); });

            out << it->points.size() << '\n';
        }
        else
        {
            throw std::runtime_error("Unknown sub-command for MAX");
        }
    }

    // MIN
    void cmd_MIN(const std::vector<Polygon> &polygons, std::istream &in, std::ostream &out)
    {
        if (polygons.empty())
        {
            throw std::runtime_error("Collection is empty");
        }

        std::string sub_cmd;
        if (!(in >> sub_cmd))
        {
            throw std::runtime_error("Invalid argument for MIN");
        }

        if (sub_cmd == "AREA")
        {
            auto it = std::min_element(polygons.begin(), polygons.end(),
                                       [](const Polygon &a, const Polygon &b)
                                       { return Geometry::get_area(a) < Geometry::get_area(b); });

            iofmtguard guard(out);
            out << std::fixed << std::setprecision(1) << Geometry::get_area(*it) << '\n';
        }
        else if (sub_cmd == "VERTICES")
        {
            auto it = std::min_element(polygons.begin(), polygons.end(),
                                       [](const Polygon &a, const Polygon &b)
                                       { return a.points.size() < b.points.size(); });

            out << it->points.size() << '\n';
        }
        else
        {
            throw std::runtime_error("Unknown sub-command for MIN");
        }
    }

    // COUNT
    void cmd_COUNT(const std::vector<Polygon> &polygons, std::istream &in, std::ostream &out)
    {
        std::string arg;
        if (!(in >> arg))
        {
            throw std::runtime_error("Invalid argument for COUNT");
        }

        long long count = 0;
        if (arg == "EVEN")
        {
            count = std::count_if(polygons.begin(), polygons.end(),
                                  [](const Polygon &p)
                                  { return p.points.size() % 2 == 0; });
        }
        else if (arg == "ODD")
        {
            count = std::count_if(polygons.begin(), polygons.end(),
                                  [](const Polygon &p)
                                  { return p.points.size() % 2 != 0; });
        }
        else
        {
            try
            {
                size_t num_vertices = std::stoul(arg);
                count = std::count_if(polygons.begin(), polygons.end(),
                                      [num_vertices](const Polygon &p)
                                      { return p.points.size() == num_vertices; });
            }
            catch (...)
            {
                throw std::runtime_error("Unknown argument for COUNT");
            }
        }
        out << count << '\n';
    }

    // для вар 2: LESSAREA
    void cmd_LESSAREA(const std::vector<Polygon> &polygons, std::istream &in, std::ostream &out)
    {
        Polygon target;
        if (!(in >> target))
        {
            throw std::runtime_error("Invalid argument for LESSAREA");
        }

        double targetArea = Geometry::get_area(target);
        auto count = std::count_if(polygons.begin(), polygons.end(),
                                   [targetArea](const Polygon &p)
                                   { return Geometry::get_area(p) < targetArea; });

        out << count << '\n';
    }

    // для вар 2: MAXSEQ
    void cmd_MAXSEQ(const std::vector<Polygon> &polygons, std::istream &in, std::ostream &out)
    {
        Polygon target;
        if (!(in >> target))
        {
            throw std::runtime_error("Invalid argument for MAXSEQ");
        }

        std::pair<int, int> start = {0, 0};
        auto result = std::accumulate(polygons.begin(), polygons.end(), start,
                                      [&target](std::pair<int, int> state, const Polygon &current)
                                      {
                                          if (current == target)
                                          {
                                              state.first += 1;
                                              state.second = std::max(state.second, state.first);
                                          }
                                          else
                                          {
                                              state.first = 0;
                                          }
                                          return state;
                                      });

        out << result.second << '\n';
    }
}
