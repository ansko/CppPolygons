#include <fstream>
#include <string>
#include <vector>

#include "plane.hpp"
#include "polygonal_cylinder.hpp"
#include "polygonal_cylinder_in_the_shell.hpp"
#include "settings_parser.hpp"

const float  PI_F = 3.14159265358979f;


void printToCSGAsCircleCylinders(std::string fname,
         std::vector<std::shared_ptr<PolygonalCylinder> > polCyl_ptrs)
{
    std::ofstream fout;
    fout.open(fname);

    SettingsParser sp("options.ini");
    sp.parseSettings();
    std::string CUBE_EDGE_LENGTH = sp.getProperty("CUBE_EDGE_LENGTH");
    std::string VERTICES_NUMBER = sp.getProperty("VERTICES_NUMBER");
    std::string THICKNESS = sp.getProperty("THICKNESS");
    std::string OUTER_RADIUS = sp.getProperty("OUTER_RADIUS");

    std::string fillerString = "solid filler = polygonalCylinder0";

    fout << "algebraic3d\n";
    fout << "solid cell = orthobrick(0, 0, 0; ";
    fout << CUBE_EDGE_LENGTH << ", "\
         << CUBE_EDGE_LENGTH << ", "\
         << CUBE_EDGE_LENGTH << ");\n";

    float verticesNumber = (float)std::stod(VERTICES_NUMBER);
    float edgeLength = (float)std::stod(OUTER_RADIUS) * 2  * sin(2 * PI_F / verticesNumber);
    float innerRadius = edgeLength / 2 / tan(PI_F / verticesNumber);

    if (polCyl_ptrs.size() > 0) {
        for (int i = 0; i < polCyl_ptrs.size(); ++i) {
            std::vector<Polygon> polygons;
            polygons.push_back(polCyl_ptrs[i]->topFacet());
            polygons.push_back(polCyl_ptrs[i]->bottomFacet());
            for (auto &facet : polCyl_ptrs[i]->facets())
                polygons.push_back(facet);
            if (i != 0)
                fillerString += " or polygonalCylinder" + std::to_string(i);
            fout << "solid polygonalCylinder" << i << " = plane(";
            for(int j = 0; j < 2; ++j) {
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
                         << pl.n().z() << ")" \
                         << " and plane(";
                else if (j == 1)
                    fout << polygon.vertices()[0].x() << ", "\
                         << polygon.vertices()[0].y() << ", "\
                         << polygon.vertices()[0].z() << "; "\
                         << -pl.n().x() << ", " \
                         << -pl.n().y() << ", " \
                         << -pl.n().z() << ")";
            }
            fout << " and cylinder(" \
                 << polygons[0].center().x() << ", "\
                 << polygons[0].center().y() << ", "\
                 << polygons[0].center().z() << "; "\
                 << polygons[1].center().x() << ", "\
                 << polygons[1].center().y() << ", "\
                 << polygons[1].center().z() << "; "\
                 << innerRadius << ");\n";
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


void printToCSGAsCircleCylindersShells(std::string fname,
         std::vector<std::shared_ptr<PolygonalCylinder> > polCyl_ptrs,
         std::vector<std::shared_ptr<PolygonalCylinder> > sh_ptrs)
{
    std::ofstream fout;
    fout.open(fname);

    SettingsParser sp("options.ini");
    sp.parseSettings();
    std::string CUBE_EDGE_LENGTH = sp.getProperty("CUBE_EDGE_LENGTH");
    std::string VERTICES_NUMBER = sp.getProperty("VERTICES_NUMBER");
    std::string THICKNESS = sp.getProperty("THICKNESS");
    std::string OUTER_RADIUS = sp.getProperty("OUTER_RADIUS");
    float sh = std::stof(sp.getProperty("SHELL_THICKNESS"));
    std::string fillerString = "solid filler = polygonalCylinder0";
    std::string shellsString = "solid shell = pc0";
    fout << "algebraic3d\n";
    fout << "solid cell = orthobrick(0, 0, 0; ";
    fout << CUBE_EDGE_LENGTH << ", "\
         << CUBE_EDGE_LENGTH << ", "\
         << CUBE_EDGE_LENGTH << ");\n";

    float verticesNumber = (float)std::stod(VERTICES_NUMBER);
    float edgeLength = (float)std::stod(OUTER_RADIUS) * 2  * sin(2 * PI_F / verticesNumber);
    float innerRadius = edgeLength / 2 / tan(PI_F / verticesNumber);

    if (polCyl_ptrs.size() > 0) {
        for (int i = 0; i < polCyl_ptrs.size(); ++i) {
            std::vector<Polygon> polygons;
            polygons.push_back(polCyl_ptrs[i]->topFacet());
            polygons.push_back(polCyl_ptrs[i]->bottomFacet());
            if (i != 0) {
                fillerString += " or polygonalCylinder" + std::to_string(i);
                shellsString += " or pc" + std::to_string(i);
            }
            fout << "solid polygonalCylinder" << i << " = plane(";
            for(int j = 0; j < 2; ++j) {
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
                         << pl.n().z() << ")" \
                         << " and plane(";
                else if (j == 1)
                    fout << polygon.vertices()[0].x() << ", "\
                         << polygon.vertices()[0].y() << ", "\
                         << polygon.vertices()[0].z() << "; "\
                         << -pl.n().x() << ", " \
                         << -pl.n().y() << ", " \
                         << -pl.n().z() << ")";
            }
            fout << " and cylinder(" \
                 << polygons[0].center().x() << ", "\
                 << polygons[0].center().y() << ", "\
                 << polygons[0].center().z() << "; "\
                 << polygons[1].center().x() << ", "\
                 << polygons[1].center().y() << ", "\
                 << polygons[1].center().z() << "; "\
                 << innerRadius << ");\n";
        }
        fillerString += " and cell;\n";
        fout << fillerString;
        for (int i = 0; i < sh_ptrs.size(); ++i) {
            std::vector<Polygon> shPolys;
            shPolys.push_back(sh_ptrs[i]->topFacet());
            shPolys.push_back(sh_ptrs[i]->bottomFacet());
            if (i != 0) {
                shellsString += " or pc" + std::to_string(i);
            }
            fout << "solid pc" << i << " = plane(";
            for(int j = 0; j < 2; ++j) {
                Polygon polygon = shPolys[j];
                Plane pl = Plane(polygon.vertices()[0],\
                                 polygon.vertices()[1],\
                                 polygon.vertices()[2]);
                if (j == 0)
                    fout << polygon.vertices()[0].x() << ", "\
                         << polygon.vertices()[0].y() << ", "\
                         << polygon.vertices()[0].z() << "; "\
                         << pl.n().x() << ", " \
                         << pl.n().y() << ", " \
                         << pl.n().z() << ")" \
                         << " and plane(";
                else if (j == 1)
                    fout << polygon.vertices()[0].x() << ", "\
                         << polygon.vertices()[0].y() << ", "\
                         << polygon.vertices()[0].z() << "; "\
                         << -pl.n().x() << ", " \
                         << -pl.n().y() << ", " \
                         << -pl.n().z() << ")";
            }
            fout << " and cylinder(" \
                 << shPolys[0].center().x() << ", "\
                 << shPolys[0].center().y() << ", "\
                 << shPolys[0].center().z() << "; "\
                 << shPolys[1].center().x() << ", "\
                 << shPolys[1].center().y() << ", "\
                 << shPolys[1].center().z() << "; "\
                 << innerRadius + sh << ");\n";
        }
        shellsString += " and cell and not filler;\n";
        fout << shellsString;
    }
    fout << "tlo filler;\n";
    fout << "tlo shell -transparent;\n";
    fout << "solid matrix = cell and not filler and not shell;\n";
    fout << "tlo matrix -transparent;\n";    
    fout.close();
    return;
};
