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
                throw std::runtime_error("Empty dataset for AREA MEAN");
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
                                             bool is_even_index = (index % 2 != 0);
                                             bool is_target_index = (is_even_index == target_even);
                                             index++;
                                             return acc + (is_target_index ? Geometry::get_area(p) : 0.0);
                                         });
            out << sum << '\n';
        }
        else
        {
            try
            {
                size_t num_vertices = std::stoul(sub_cmd);
                if (num_vertices < 3)
                {
                    throw std::runtime_error("Invalid vertex count");
                }

                double sum = std::accumulate(polygons.begin(), polygons.end(), 0.0,
                                             [num_vertices](double acc, const Polygon &p)
                                             {
                                                 return acc + (p.points.size() == num_vertices ? Geometry::get_area(p) : 0.0);
                                             });
                out << sum << '\n';
            }
            catch (...)
            {
                throw std::runtime_error("Unknown sub-command for AREA");
            }
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
        else if (sub_cmd == "VERTEXES")
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
        else if (sub_cmd == "VERTEXES")
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
                if (num_vertices < 3)
                {
                    throw std::runtime_error("Invalid vertex count");
                }
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
}