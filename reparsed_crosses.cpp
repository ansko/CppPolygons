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
    int n = (int)std::stod(sp.getProperty("VERTICES_NUMBER"));
    int N = (int)std::stod(sp.getProperty("DISKS_NUM"));
    int cube_edge = (int)std::stod(sp.getProperty("CUBE_EDGE_LENGTH"));
    Point vertex;
    std::vector<Point> poly_vertices; // all points from file
    std::vector<std::shared_ptr<PolygonalCylinder> > pc_ptrs;
    float vertex_x, vertex_y, vertex_z;
    std::ifstream fin;
    std::ofstream fout;
    std::vector<std::string> taus;
    taus.push_back("1.5");
    taus.push_back("2.5");
    taus.push_back("5");
    for (uint par_N = 1; par_N < 9; ++par_N) {
        uint particles_range;
        std::string particles_number;
        if (par_N == 8) {
           particles_range = 30;
           particles_number = std::to_string(30);
        }
        else {
           particles_range = par_N * 4;
           particles_number = std::to_string(par_N * 4);
        }
        for (uint i_tau = 0; i_tau < 3; ++i_tau) {
            for (uint attempt = 0; attempt < 5; ++attempt) {
                std::string file_in = "Reparsed/tau" + taus[i_tau] +
                                      std::string("N") + particles_number + "_" + 
                                      std::to_string(attempt) + ".geo";
                std::string file_out = "Crosses/tau" + taus[i_tau] +
                                      std::string("N") + particles_number + "_" + 
                                      std::to_string(attempt);
                std::cout << file_in << " " << file_out << std::endl;
                fin.open(file_in);
                fout.open(file_out);
                for (uint particle_num = 0;
                          particle_num < particles_range; ++particle_num) {
                    std::string facet_name;
                    std::shared_ptr<Polygon> top_facet_ptr, bottom_facet_ptr;
                    poly_vertices.clear();
                    for (uint side = 0; side < n; side++) {
                        fin >> facet_name >> vertex_x >> vertex_y >> vertex_z;
                        if (facet_name != std::string("top")) {
                            std::cout << "error in facet name: " << facet_name
                                << " but shoud be top\n";
                            return 0;
                        }
                        poly_vertices.push_back(
                            Point(vertex_x, vertex_y, vertex_z));
                    }
                    top_facet_ptr = std::make_shared<Polygon>(poly_vertices);
                    poly_vertices.clear();
                    for (uint side = 0; side < n; side++) {
                        fin >> facet_name >> vertex_x >> vertex_y >> vertex_z;
                        if (facet_name != std::string("bottom")) {
                            std::cout << "error in facet name: " << facet_name
                                << " but shoud be bottom\n";
                            return 0;
                        }
                        poly_vertices.push_back(
                            Point(vertex_x, vertex_y, vertex_z));
                    }
                    bottom_facet_ptr = std::make_shared<Polygon>(poly_vertices);
                    pc_ptrs.push_back(
                        std::make_shared<PolygonalCylinder>(
                            top_facet_ptr, bottom_facet_ptr));
                }
                std::vector<std::vector<int> > crosses;
                for (uint i = 0; i < particles_range; ++i) {
                    std::vector<int> tmp;
                    for (uint j = 0; j < particles_range; j++)
                        tmp.push_back(int(0));
                    crosses.push_back(tmp);
                }
                for (uint i = 0; i < particles_range; i++) {
                    for (uint j = 0; j < particles_range; j++) {
                        if (i == j)
                            continue;
                        auto pci = *pc_ptrs[i];
                        auto pcj = *pc_ptrs[j];
                        if (pci.crossesOtherPolygonalCylinder(pcj, 0)) {
                            crosses[i][j] = 1;
                            for (uint ip = 0; ip < n; ++ip) {
                                for (uint jp = 0; jp < n; ++jp) {
                                    Polygon poly1 = pci.facets()[ip];
                                    Polygon poly2 = pcj.facets()[jp];
                                }
                            }
                        }
                    }
                }
//                float minx = cude_edge, miny = cude_edge, minz = cude_edge,
//                      maxx = 0, maxy = 0, maxz = 0;
                for (uint i = 0; i < particles_range; ++i) {
                    fout << i << ":";
                    for (uint j = 0; j < particles_range; ++j)
                        if (crosses[i][j] == 1)
                    fout << j << ":";
                    fout << std::endl;
                }
                fout.close();
                fin.close();
            }
        }
    }
    return 0;
}
