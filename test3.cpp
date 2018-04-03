#include <iostream>
#include <memory>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "include/settings_parser.hpp"
#include "include/csg_printer_circles.hpp"
#include "include/csg_printer_polygons.hpp"
#include "include/geometries/point.hpp"
#include "include/geometries/polygonal_cylinder.hpp"
#include "include/ng_errors_checkers/checker_parallel_surfaces.hpp"
#include "include/percolation/percolation_checker.hpp"


int main(int argc, char **argv)
{
    bool debug_flag = true;
    std::cout << "--polygonal_start--\n";
    // parsing settings
    SettingsParser sp("tmp/options.ini");
    sp.parseSettings();
    int n = 6;//(int)std::stod(sp.getProperty("VERTICES_NUMBER"));
    int N = 4;//(int)std::stod(sp.getProperty("DISKS_NUM"));
    Point vertex;
    std::vector<Point> poly_vertices; // all points from file
    std::vector<std::shared_ptr<PolygonalCylinder> > pc_ptrs;
    float vertex_x, vertex_y, vertex_z;
    std::ifstream fin;
    fin.open("1.geo");

    int particles_num = 4;

    for (uint str_num = 0; str_num < particles_num * 9 + 2; ++str_num) {
        if (str_num < 3)
            continue;
        switch ((str_num - 2) % 9) {
            case 1: break;
            case 2: {
                std::string s;
                fin >> s;
                std::cout << s;
            }
        }
    }

    return 0;
}
