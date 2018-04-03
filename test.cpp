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
    fin.open("reparsed");
    std::cout << "reading reparsed\n";
    for (uint particle_num = 0; particle_num < N; ++particle_num) {
        std::string facet_name;
        std::shared_ptr<Polygon> top_facet_ptr, bottom_facet_ptr;
        poly_vertices.clear();
        // top
        for (uint side = 0; side < n; side++) {
            fin >> facet_name >> vertex_x >> vertex_y >> vertex_z;
            if (facet_name != std::string("top")) {
                std::cout << "error in facet name: " << facet_name
                          << " but shoud be top\n";
                return 0;
            }
            poly_vertices.push_back(Point(vertex_x, vertex_y, vertex_z));
        }
        top_facet_ptr = std::make_shared<Polygon>(poly_vertices);
        poly_vertices.clear();
        // bottom
        for (uint side = 0; side < n; side++) {
            fin >> facet_name >> vertex_x >> vertex_y >> vertex_z;
            if (facet_name != std::string("bottom")) {
                std::cout << "error in facet name: " << facet_name
                          << " but shoud be bottom\n";
                return 0;
            }
            poly_vertices.push_back(Point(vertex_x, vertex_y, vertex_z));
        }
        bottom_facet_ptr = std::make_shared<Polygon>(poly_vertices);
        pc_ptrs.push_back(std::make_shared<PolygonalCylinder>(top_facet_ptr, bottom_facet_ptr));
    }
    std::cout << "finished reading\n";
    /*
    for (uint i = 0; i < pc_ptrs.size(); ++i) {
        std::cout << pc_ptrs[i]->facets()[0].vertices().size() << std::endl;
    }
    return 0;
    */

    std::cout << "checking intersections\n";
    for (uint i = 0; i < pc_ptrs.size(); ++i) {
        for (uint j = 0; j < pc_ptrs.size(); ++j) {
            if (i == j)
                continue;
            PolygonalCylinder pc1 = *pc_ptrs[i], pc2 = *pc_ptrs[j];
            bool intersection_flag = pc1.crossesOtherPolygonalCylinder(pc2, 0);
            std::cout << i << " " << j << " " << intersection_flag << std::endl;
        }
    }


    return 0;
}
