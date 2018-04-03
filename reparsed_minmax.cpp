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
                std::string file_out = "ParticleRanges/tau" + taus[i_tau] +
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
                for (uint i = 0; i < particles_range; i++) {
                    float minx = cube_edge, miny = cube_edge, minz = cube_edge,
                          maxx = 0, maxy = 0, maxz = 0;
                    auto pc = pc_ptrs[i];
                    auto top_facet = pc->topFacet();
                    auto bottom_facet = pc->bottomFacet();
                    for (auto v : top_facet.vertices()) {
                        if (v.x() < minx)
                            minx = v.x();
                        if (v.y() < miny)
                            miny = v.y();
                        if (v.z() < minz)
                            minz = v.z();
                        if (v.x() > maxx)
                            maxx = v.x();
                        if (v.y() > maxy)
                            maxy = v.y();
                        if (v.z() > maxz)
                            maxz = v.z();
                    }
                    for (auto v : top_facet.vertices()) {
                        if (v.x() < minx)
                            minx = v.x();
                        if (v.y() < miny)
                            miny = v.y();
                        if (v.z() < minz)
                            minz = v.z();
                        if (v.x() > maxx)
                            maxx = v.x();
                        if (v.y() > maxy)
                            maxy = v.y();
                        if (v.z() > maxz)
                            maxz = v.z();
                    }
                    fout << i << ":" << minx << ":" << miny << ":" << minz
                              << ":" << maxx << ":" << maxy << ":" << maxz
                              << std::endl;
                }
                fout.close();
                fin.close();
            }
        }
    }
    return 0;
}
