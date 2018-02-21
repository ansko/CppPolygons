#include <iostream>
#include <memory>
#include <vector>
#include <ctime>
#include <cstdlib>

#include "include/settings_parser.hpp"
#include "include/csg_printer_circles.hpp"
#include "include/csg_printer_polygons.hpp"
#include "include/geometries/polygonal_cylinder.hpp"
#include "include/percolation/percolation_checker.hpp"


int main(int argc, char **argv)
{
    std::cout << "--polygonal_start--\n";
    // parsing settings
    SettingsParser sp("tmp/options.ini");
    sp.parseSettings();
    float cubeSize = (float)std::stod(sp.getProperty("CUBE_EDGE_LENGTH"));
    int n = (int)std::stod(sp.getProperty("VERTICES_NUMBER"));
    float h = (float)std::stod(sp.getProperty("THICKNESS"));
    float sh = (float)std::stod(sp.getProperty("SHELL_THICKNESS"));
    float interlayer = (float)std::stod(sp.getProperty("INTERLAYER"));
    float R = (float)std::stod(sp.getProperty("OUTER_RADIUS"));
    int N = (int)std::stod(sp.getProperty("DISKS_NUM"));
    int TN = (int)std::stod(sp.getProperty("TACTOID_NUM"));
    int MAX_ATTEMPTS = (int)std::stod(sp.getProperty("MAX_ATTEMPTS"));
    std::string FNAME = sp.getProperty("FNAME");
    std::string PERC_FNAME = sp.getProperty("PERC_FNAME");
    float edgeLength = R * 2  * sin(PI_F / n);
    float innerRadius = edgeLength / 2 / tan(PI_F / n);
    float r = R * cos(PI_F / n);
    std::cout << "**parameters_start**" << std::endl
              << "AR = " << 2 * r /h << std::endl
              << "MAX_ATTEMPTS = " << MAX_ATTEMPTS << std::endl
              << "number of filler particles = " << N << std::endl
              << "inner radius = " << r << std::endl
              << "thickness = " << h << std::endl
              << "shell thickness = " << sh << std::endl
              << "cube edge length = " << cubeSize << std::endl
              << "**parameters_end**" << std::endl;
    // starting to create initial configuration
    std::vector<std::shared_ptr<PolygonalCylinder> > polCyls;
    std::vector<std::shared_ptr<PolygonalCylinder> > shells;
    int attempt = 0;
    srand(time(NULL));
    while(polCyls.size() < N && ++attempt < MAX_ATTEMPTS) {
        if (attempt % (MAX_ATTEMPTS / 10) == 0)
            std::cout << attempt << std::endl;
        std::vector<std::shared_ptr<PolygonalCylinder> > newCyls;
        std::vector<std::shared_ptr<PolygonalCylinder> > newShells;
        for (uint i = 0; i < TN; ++i) {
            std::shared_ptr<PolygonalCylinder> polCyl_ptr =
                std::make_shared<PolygonalCylinder>(n, h, R);
            std::shared_ptr<PolygonalCylinder> sh_ptr =
                std::make_shared<PolygonalCylinder>(n, h + 2 * sh, R + sh);
            polCyl_ptr->translate(0, 0, interlayer * i + h * i + h / 2);
            sh_ptr->translate(0, 0, interlayer * i + h * i + h / 2);
            newCyls.push_back(polCyl_ptr);
            newShells.push_back(sh_ptr);
        }
        float dx = static_cast<float>(rand()) / RAND_MAX * cubeSize;
        float dy = static_cast<float>(rand()) / RAND_MAX * cubeSize;
        float dz = static_cast<float>(rand()) / RAND_MAX * cubeSize;
        float alpha = static_cast<float>(rand()) / RAND_MAX;
        float beta = static_cast<float>(rand()) / RAND_MAX;
        float gamma = static_cast<float>(rand()) / RAND_MAX;
        for (auto polCyl_ptr : newCyls) {
            polCyl_ptr->rotateAroundX(PI_F * alpha);
            polCyl_ptr->rotateAroundY(PI_F * beta);
            polCyl_ptr->rotateAroundZ(PI_F * gamma);
            polCyl_ptr->translate(dx, dy, dz);
        }
        for (auto sh_ptr : newShells) {
            sh_ptr->rotateAroundX(PI_F * alpha);
            sh_ptr->rotateAroundY(PI_F * beta);
            sh_ptr->rotateAroundZ(PI_F * gamma);
            sh_ptr->translate(dx, dy, dz);
        }
        bool flag_append = true;
        for (auto polCyl_ptr : newCyls) {
            if (polCyl_ptr->crossesBox(cubeSize)) {
                flag_append = false;
                break;
            }

            for(auto oldPc : polCyls)
                if (polCyl_ptr->crossesOtherPolygonalCylinder(*oldPc, 0)) {
                    flag_append = false;
                    break;
                }
        }
        if(!flag_append)
            continue;
        for (auto polCyl_ptr : newCyls)
            polCyls.push_back(polCyl_ptr);
        for (auto sh_ptr : newShells)
            shells.push_back(sh_ptr);
    } 
    float pcVolume =  PI_F * pow(r, 2) * h;
    float shVolume =  PI_F * pow(r + sh, 2) * (h + 2 * sh);
    float cubeVolume = pow(cubeSize, 3);
    std::cout << "volume fraction of filler = "
              << polCyls.size() * pcVolume / cubeVolume << std::endl;
    std::cout << "CylsNum = " << polCyls.size() << ", ";
    std::cout << "Attempts = " << attempt << std::endl;
//    std::shared_ptr<CSGPrinterCircles> printer_ptr;
//    printer_ptr->printToCSGAsCircleCylindersShells(FNAME, polCyls, shells);
    std::shared_ptr<CSGPrinterPolygons> printer_ptr;
    printer_ptr->printToCSGAsPolygonalCylindersShells(FNAME, polCyls, shells);
    PercolationChecker pc(shells); 
    pc.sideToSide();
    std::cout << "--polygonal_end--\n";
    return 0;
}
