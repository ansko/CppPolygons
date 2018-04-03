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
        /*{
            std::cout << "--- TOPS ---\n";
            for (uint side = 0; side < n; side++)
                fin >> facet_name >> vertex_x >> vertex_y >> vertex_z;
        }*/
        bottom_facet_ptr = std::make_shared<Polygon>(poly_vertices);
        pc_ptrs.push_back(std::make_shared<PolygonalCylinder>(top_facet_ptr, bottom_facet_ptr));
    }

    std::vector<std::vector<int> > crosses;
    for (uint i = 0; i < N; ++i) {
        std::vector<int> tmp;
        for (uint j = 0; j < N; j++)
            tmp.push_back(int(0));
        crosses.push_back(tmp);
    }

    // checking intersections
    std::cout << "\n\n\n";
    {
        for (uint i = 0; i < N; i++) {
            for (uint j = 0; j < N; j++) {
                if (i == j)
                    continue;
                //std::cout << "checking: " << i << " " << j << std::endl;
                // direct implementation of intersection function
                {
                    auto pci = *pc_ptrs[i];
                    auto pcj = *pc_ptrs[j];
                    if (pci.crossesOtherPolygonalCylinder(pcj, 0)) {
                        //std::cout << "crossing! more accurate check...\n";
                        // exploring what facets cross:
                        crosses[i][j] = 1;

/*                        for (uint ip = 0; ip < n; ++ip) {
                            for (uint jp = 0; jp < n; ++jp) {
                                //std::cout << pci.facets().size() << std::endl;
                                Polygon poly1 = pci.facets()[ip];
                                Polygon poly2 = pcj.facets()[jp];
                                if (poly1.crossesOtherPolygon(poly2)) {
                                    std::cout << "polygons cross: " << ip << " " << jp << std::endl;
                                    //std::cout << "poly1" << ip << "\n";
                                    //for (auto v : poly1.vertices())
                                    //    std::cout << v.x() << " " << v.y() << " " << v.z() << std::endl;
                                    //std::cout << "poly2-" << jp << "\n";
                                    //for (auto v : poly2.vertices())
                                    //    std::cout << v.x() << " " << v.y() << " " << v.z() << std::endl;
                                }
                            }
                        }
*/
                    }
                    //std::cout << "\n";
                }
            }
        }
    }

    for (uint i = 0; i < N; ++i) {
        std::cout << i << ": ";
        for (uint j = 0; j < N; ++j)
            if (crosses[i][j] == 1)
                std::cout << j << " ";
        std::cout << std::endl;
    }

    //std::cout << "general facets number: " << pc_ptrs[0]->facets().size() << std::endl;

    return 0;
}
