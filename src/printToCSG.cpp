#include <fstream>
#include <string>
#include <vector>

#include "plane.hpp"
#include "polygonal_cylinder.hpp"

#define CUBE_EDGE_LENGTH 20.
#define MAX_ATTEMPTS 50000.
#define DISKS_NUM 10.
#define VERTICES_NUMBER 8.
#define THICKNESS 1.
#define OUTER_RADIUS 5.
#define FNAME "1.geo"

void printToCSG(std::string fname, std::vector<PolygonalCylinder> pcs) {
    std::ofstream fout;
    fout.open(fname);

    std::string fillerString = "solid filler = polygonalCylinder0";

    fout << "algebraic3d\n";
    fout << "solid cell = orthobrick(0, 0, 0; ";
    fout << CUBE_EDGE_LENGTH << ", "\
         << CUBE_EDGE_LENGTH << ", "\
         << CUBE_EDGE_LENGTH << ");\n";
    
    if (pcs.size() > 0) {
        for (int i = 0; i < pcs.size(); ++i) {
            std::vector<Polygon> polygons;
            polygons.push_back(pcs[i].topFacet());
            polygons.push_back(pcs[i].bottomFacet());
            for (auto &facet : pcs[i].facets())
                polygons.push_back(facet);
            if (i != 0)
                fillerString += " or polygonalCylinder" + std::to_string(i);
            fout << "solid polygonalCylinder" << i << " = plane(";
            for(int j = 0; j < polygons.size(); ++j) {
                Polygon polygon = polygons[j];
                Plane pl = Plane(polygon.vertices()[0],\
                                 polygon.vertices()[1],\
                                 polygon.vertices()[2]);
                if (j == 0)
                    fout << polygon.vertices()[0].x() << ", "\
                         << polygon.vertices()[0].y() << ", "\
                         << polygon.vertices()[0].z() << "; "\
                         << pl.n().x() << ", " \
                         << pl.n().y() << ", " \
                         << pl.n().z() << ")";
                else if (j == 1)
                    fout << polygon.vertices()[0].x() << ", "\
                         << polygon.vertices()[0].y() << ", "\
                         << polygon.vertices()[0].z() << "; "\
                         << -pl.n().x() << ", " \
                         << -pl.n().y() << ", " \
                         << -pl.n().z() << ")";
                else
                    fout << polygon.vertices()[0].x() << ", "\
                         << polygon.vertices()[0].y() << ", "\
                         << polygon.vertices()[0].z() << "; "\
                         << -pl.n().x() << ", " \
                         << -pl.n().y() << ", " \
                         << -pl.n().z() << ")";
                if (j != polygons.size() - 1)
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
