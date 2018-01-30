#include <fstream>
#include <string>
#include <vector>

#include "plane.hpp"
#include "polygonal_cylinder.hpp"
#include "polygonal_cylinder_in_the_shell.hpp"
#include "settings_parser.hpp"

const float  PI_F = 3.14159265358979f;

void printToCSG(std::string fname, std::vector<PolygonalCylinder> pcs) {
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


void printToCSG(std::string fname, 
                std::vector<std::shared_ptr<PolygonalCylinder> > pc_ptrs) {
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
    
    if (pc_ptrs.size() > 0) {
        for (int i = 0; i < pc_ptrs.size(); ++i) {
            std::vector<Polygon> polygons;
            polygons.push_back((*pc_ptrs[i]).topFacet());
            polygons.push_back((*pc_ptrs[i]).bottomFacet());
            for (auto &facet : (*pc_ptrs[i]).facets())
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


void printToCSG(std::string fname,
                std::vector<std::shared_ptr<PolygonalCylinderInTheShell> > pcitss) {
    std::ofstream fout;
    fout.open(fname);

    std::cout << "pcitss\n";

    SettingsParser sp("options.ini");
    sp.parseSettings();
    std::string CUBE_EDGE_LENGTH = sp.getProperty("CUBE_EDGE_LENGTH");
    std::string VERTICES_NUMBER = sp.getProperty("VERTICES_NUMBER");
    std::string THICKNESS = sp.getProperty("THICKNESS");
    float SHELL_THICKNESS = (float)std::stod(sp.getProperty("SHELL_THICKNESS"));
    float OUTER_RADIUS = (float)std::stod(sp.getProperty("OUTER_RADIUS"));

//    std::string fillerString = "solid filler = (polygonalCylinder0";
    std::string fillerString = "solid filler = polygonalCylinder0";

    fout << "algebraic3d\n";
    fout << "solid cell = orthobrick(0, 0, 0; ";
    fout << CUBE_EDGE_LENGTH << ", "\
         << CUBE_EDGE_LENGTH << ", "\
         << CUBE_EDGE_LENGTH << ");\n";
    
    if (pcitss.size() > 0) {
        float centralAngle = PI_F / pcitss[0]->facets()[0].vertices().size();
        for (int i = 0; i < pcitss.size(); ++i) {
            std::vector<Polygon> polygons;
            polygons.push_back(pcitss[i]->topFacet());
            polygons.push_back(pcitss[i]->bottomFacet());
            for (auto &facet : pcitss[i]->facets())
                polygons.push_back(facet);
            if (i != 0)
                fillerString += " or polygonalCylinder" + std::to_string(i);
            fout << "solid polygonalCylinder" << i << " = plane(";
            Point pcitsc;
            Vector vpcitsc = Vector(pcitss[i]->topFacet().center().x() / 2 +
                                    pcitss[i]->bottomFacet().center().x() / 2,
                                    pcitss[i]->topFacet().center().y() / 2 +
                                    pcitss[i]->bottomFacet().center().y() / 2,
                                    pcitss[i]->topFacet().center().z() / 2 +
                                    pcitss[i]->bottomFacet().center().z() / 2);
            pcitsc = Point(vpcitsc.x(), vpcitsc.y(), vpcitsc.z());
            for(int j = 0; j < polygons.size(); ++j) {
                Polygon polygon = polygons[j];
                Point polyc = polygon.center();
                Vector vpolyc0 = Vector(polyc, polygon.vertices()[0]);
                Vector vpolyc1 = Vector(polyc, polygon.vertices()[1]);
                Vector vpolyc2 = Vector(polyc, polygon.vertices()[2]);
                Vector vpcitscpolyc(pcitsc, polyc);
                float l = vpcitscpolyc.length();
                vpcitscpolyc = vpcitscpolyc * (l + SHELL_THICKNESS) / l;
                Vector v0 = vpcitsc + vpcitscpolyc + vpolyc0;
                Vector v1 = vpcitsc + vpcitscpolyc + vpolyc1;
                Vector v2 = vpcitsc + vpcitscpolyc + vpolyc2;
                Point pt0 = Point(v0.x(), v0.y(), v0.z());
                Point pt1 = Point(v1.x(), v1.y(), v1.z());
                Point pt2 = Point(v2.x(), v2.y(), v2.z());
                Plane pl(pt0, pt1, pt2);
                if (j == 0)
                    fout << pt0.x() << ", "
                         << pt0.y() << ", "
                         << pt0.z() << "; "
                         << pl.n().x() << ", "
                         << pl.n().y() << ", "
                         << pl.n().z() << ")";
                else if (j == 1)
                    fout << pt0.x() << ", "
                         << pt0.y() << ", "
                         << pt0.z() << "; "
                         << -pl.n().x() << ", "
                         << -pl.n().y() << ", "
                         << -pl.n().z() << ")";
                else
                    fout << pt0.x() << ", "
                         << pt0.y() << ", "
                         << pt0.z() << "; "
                         << -pl.n().x() << ", "
                         << -pl.n().y() << ", "
                         << -pl.n().z() << ")";
                if (j != polygons.size() - 1)
                    fout << " and plane(";
                else
                    fout << ";\n";
            }
        }
        //fillerString += "} and cell;\n";
        fillerString += ");\n";
        fout << fillerString;
    }
    fout << "tlo filler;\n";
    fout << "solid matrix = cell and not filler;\n";
    fout << "tlo matrix -transparent;\n";    
    fout.close();
    return;
};


void printToCSG(std::string fname,
                std::vector<PolygonalCylinderInTheShell> pcitss,
                int mode) {
    std::ofstream fout;
    fout.open(fname);

    SettingsParser sp("options.ini");
    sp.parseSettings();
    std::string CUBE_EDGE_LENGTH = sp.getProperty("CUBE_EDGE_LENGTH");
    std::string VERTICES_NUMBER = sp.getProperty("VERTICES_NUMBER");
    std::string THICKNESS = sp.getProperty("THICKNESS");
    float SHELL_THICKNESS = (float)std::stod(sp.getProperty("SHELL_THICKNESS"));
    float OUTER_RADIUS = (float)std::stod(sp.getProperty("OUTER_RADIUS"));

    std::string fillerString = "solid filler = polygonalCylinder0";

    fout << "algebraic3d\n";
    fout << "solid cell = orthobrick(0, 0, 0; ";
    fout << CUBE_EDGE_LENGTH << ", "\
         << CUBE_EDGE_LENGTH << ", "\
         << CUBE_EDGE_LENGTH << ");\n";
    
    if (pcitss.size() > 0) {
        float centralAngle = PI_F / pcitss[0].facets()[0].vertices().size();
        for (int i = 0; i < pcitss.size(); ++i) {
            std::vector<Polygon> polygons;
            polygons.push_back(pcitss[i].topFacet());
            polygons.push_back(pcitss[i].bottomFacet());
            for (auto &facet : pcitss[i].facets())
                polygons.push_back(facet);
            if (i != 0)
                fillerString += " or polygonalCylinder" + std::to_string(i);
            fout << "solid polygonalCylinder" << i << " = plane(";
            Point pcitsc;
            Vector vpcitsc = Vector(pcitss[i].topFacet().center().x() / 2 +
                                    pcitss[i].bottomFacet().center().x() / 2,
                                    pcitss[i].topFacet().center().y() / 2 +
                                    pcitss[i].bottomFacet().center().y() / 2,
                                    pcitss[i].topFacet().center().z() / 2 +
                                    pcitss[i].bottomFacet().center().z() / 2);
            pcitsc = Point(vpcitsc.x(), vpcitsc.y(), vpcitsc.z());
            for(int j = 0; j < polygons.size(); ++j) {
                Polygon polygon = polygons[j];
                Point polyc = polygon.center();
                Vector vpolyc0 = Vector(polyc, polygon.vertices()[0]);
                Vector vpolyc1 = Vector(polyc, polygon.vertices()[1]);
                Vector vpolyc2 = Vector(polyc, polygon.vertices()[2]);
                Vector vpcitscpolyc(pcitsc, polyc);
                float l = vpcitscpolyc.length();
                vpcitscpolyc = vpcitscpolyc * (l + SHELL_THICKNESS) / l;
                Vector v0 = vpcitsc + vpcitscpolyc + vpolyc0;
                Vector v1 = vpcitsc + vpcitscpolyc + vpolyc1;
                Vector v2 = vpcitsc + vpcitscpolyc + vpolyc2;
                Point pt0 = Point(v0.x(), v0.y(), v0.z());
                Point pt1 = Point(v1.x(), v1.y(), v1.z());
                Point pt2 = Point(v2.x(), v2.y(), v2.z());
                Plane pl(pt0, pt1, pt2);
                if (j == 0)
                    fout << pt0.x() << ", "
                         << pt0.y() << ", "
                         << pt0.z() << "; "
                         << pl.n().x() << ", "
                         << pl.n().y() << ", "
                         << pl.n().z() << ")";
                else if (j == 1)
                    fout << pt0.x() << ", "
                         << pt0.y() << ", "
                         << pt0.z() << "; "
                         << -pl.n().x() << ", "
                         << -pl.n().y() << ", "
                         << -pl.n().z() << ")";
                else
                    fout << pt0.x() << ", "
                         << pt0.y() << ", "
                         << pt0.z() << "; "
                         << -pl.n().x() << ", "
                         << -pl.n().y() << ", "
                         << -pl.n().z() << ")";
                if (j != polygons.size() - 1)
                    fout << " and plane(";
                else
                    fout << ";\n";
            }
        }
        //fillerString += ") and cell;\n";
        fillerString += ";\n";
        fout << fillerString;
    }
    fout << "tlo filler;\n";
    fout << "solid matrix = cell and not filler;\n";
    fout << "tlo matrix -transparent;\n";    
    fout.close();
    return;
};
