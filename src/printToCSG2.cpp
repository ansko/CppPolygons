#include <fstream>
#include <string>
#include <vector>

#include "plane.hpp"
#include "polygonal_cylinder.hpp"
#include "polygonal_cylinder_in_the_shell.hpp"
#include "settings_parser.hpp"

const float  PI_F = 3.14159265358979f;

void printToCSG2(std::string fname, std::vector<std::shared_ptr<PolygonalCylinder> > pc_ptrs) {
    std::ofstream fout;
    fout.open(fname);

    std::shared_ptr<SettingsParser> sp_ptr = std::make_shared<SettingsParser>("options.ini");
    sp_ptr->parseSettings();
    std::string CUBE_EDGE_LENGTH = sp_ptr->getProperty("CUBE_EDGE_LENGTH");
    std::string VERTICES_NUMBER = sp_ptr->getProperty("VERTICES_NUMBER");
    std::string THICKNESS = sp_ptr->getProperty("THICKNESS");
    std::string OUTER_RADIUS = sp_ptr->getProperty("OUTER_RADIUS");

    std::string fillerString = "solid filler = polygonalCylinder0";

    fout << "algebraic3d\n";
    fout << "solid cell = orthobrick(0, 0, 0; ";
    fout << CUBE_EDGE_LENGTH << ", "\
         << CUBE_EDGE_LENGTH << ", "\
         << CUBE_EDGE_LENGTH << ");\n";
    
    if (pc_ptrs.size() > 0) {
        for (int i = 0; i < pc_ptrs.size(); ++i) {
            std::vector<std::shared_ptr<Polygon> > polygon_ptrs;
            polygon_ptrs.push_back(std::make_shared<Polygon>(pc_ptrs[i]->topFacet()));
            polygon_ptrs.push_back(std::make_shared<Polygon>(pc_ptrs[i]->bottomFacet()));
            for (auto &facet : pc_ptrs[i]->facets())
                polygon_ptrs.push_back(std::make_shared<Polygon>(facet));
            if (i != 0)
                fillerString += " or polygonalCylinder" + std::to_string(i);
            fout << "solid polygonalCylinder" << i << " = plane(";
            for(int j = 0; j < polygon_ptrs.size(); ++j) {
                std::shared_ptr<Plane> pl_ptr = std::make_shared<Plane>(polygon_ptrs[j]->vertices()[0],
                                                                        polygon_ptrs[j]->vertices()[1],
                                                                        polygon_ptrs[j]->vertices()[2]);
                if (j == 0)
                    fout << polygon_ptrs[j]->vertices()[0].x() << ", "
                         << polygon_ptrs[j]->vertices()[0].y() << ", "
                         << polygon_ptrs[j]->vertices()[0].z() << "; "
                         << pl_ptr->n().x() << ", "
                         << pl_ptr->n().y() << ", "
                         << pl_ptr->n().z() << ")";
                else if (j == 1)
                    fout << polygon_ptrs[j]->vertices()[0].x() << ", "\
                         << polygon_ptrs[j]->vertices()[0].y() << ", "\
                         << polygon_ptrs[j]->vertices()[0].z() << "; "\
                         << -pl_ptr->n().x() << ", " \
                         << -pl_ptr->n().y() << ", " \
                         << -pl_ptr->n().z() << ")";
                else
                    fout << polygon_ptrs[j]->vertices()[0].x() << ", "\
                         << polygon_ptrs[j]->vertices()[0].y() << ", "\
                         << polygon_ptrs[j]->vertices()[0].z() << "; "\
                         << -pl_ptr->n().x() << ", " \
                         << -pl_ptr->n().y() << ", " \
                         << -pl_ptr->n().z() << ")";
                if (j != polygon_ptrs.size() - 1)
                    fout << " and plane(";
                else
                    fout << ";\n";
            }
        }
        fillerString += " and cell;\n";
        fout << fillerString;
    }
    fout << "tlo filler;\n";
    fout << "solid matrix = cell and not filler;\n";
    fout << "tlo matrix -transparent;\n";    
    fout.close();
    return;
};
