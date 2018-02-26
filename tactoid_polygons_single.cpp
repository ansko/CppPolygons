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
    int ms = (int)std::stod(sp.getProperty("MIXING_STEPS"));
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
    {
        std::vector<std::shared_ptr<PolygonalCylinder> > newCyls;
        std::vector<std::shared_ptr<PolygonalCylinder> > newShells;
        for (uint i = 0; i < TN; ++i) {
            std::shared_ptr<PolygonalCylinder> polCyl_ptr =
                std::make_shared<PolygonalCylinder>(n, h, R);
            std::shared_ptr<PolygonalCylinder> sh_ptr =
                std::make_shared<PolygonalCylinder>(n, h + 2 * sh, R + sh);
            polCyl_ptr->rotateAroundZ(0.1 * i);
            sh_ptr->rotateAroundZ(0.1 * i);
            polCyl_ptr->translate(cubeSize / 2,
                                  cubeSize / 2,
                                  interlayer * i + h * i + h + sh);
            sh_ptr->translate(cubeSize / 2,
                              cubeSize / 2,
                              interlayer * i + h * i + h + sh);
            newCyls.push_back(polCyl_ptr);
            newShells.push_back(sh_ptr);
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
        for (auto polCyl_ptr : newCyls)
            polCyls.push_back(polCyl_ptr);
        for (auto sh_ptr : newShells)
            shells.push_back(sh_ptr);
    }
    {
        std::shared_ptr<CSGPrinterPolygons> printer_ptr;
        printer_ptr->printToCSGAsPolygonalCylindersShells(std::string("geofiles/start.geo"),
                                                          polCyls, shells);
    }
    float pcVolume =  PI_F * pow(r, 2) * h;
    float shVolume =  PI_F * pow(r + sh, 2) * (h + 2 * sh);
    float cubeVolume = pow(cubeSize, 3);
    std::cout << "volume fraction of filler = "
              << polCyls.size() * pcVolume / cubeVolume << std::endl;
    std::cout << "CylsNum = " << polCyls.size() << ", ";
    std::cout << "Attempts = " << attempt << std::endl;

    // simulating mixing
    std::cout << "Start mixing\n";
    uint cyls_size = polCyls.size();
    int mixing_step_attempt = 0;
    for (uint i = 0; i < ms; ++i) {
        std::cout << "Mixing, step = " << i << std::endl;
        for (uint j = 0; j < cyls_size; ++j) {
            std::shared_ptr<PolygonalCylinder> cyl_ptr = polCyls[j];
            float small_alpha = PI_F * (static_cast<float>(rand()) / RAND_MAX) / 100;
            float small_beta = PI_F * (static_cast<float>(rand()) / RAND_MAX) / 100;
            float small_gamma = PI_F * (static_cast<float>(rand()) / RAND_MAX) / 100;
            float small_dx = (static_cast<float>(rand()) / RAND_MAX - 0.5) / 10;
            float small_dy = (static_cast<float>(rand()) / RAND_MAX - 0.5) / 10;
            float small_dz = (static_cast<float>(rand()) / RAND_MAX - 0.5) / 10;
            bool flag_transformation_done = true;
            /*small_alpha = 0;
            small_beta = 0;
            small_gamma = 0;
            small_dx = 0;
            small_dy = 0;
            small_dz = 0;*/
            /*std::cout << "dx, dy, dz, alpha, beta, gamma = "
                      << small_dx << " " << small_dy << " " << small_dz << " "
                      << small_alpha << " " << small_beta << " " << small_gamma << std::endl;*/
            cyl_ptr->rotateAroundX(small_alpha);
            cyl_ptr->rotateAroundY(small_beta);
            cyl_ptr->rotateAroundZ(small_gamma);
            cyl_ptr->translate(small_dx, small_dy, small_dz);
            /*std::cout << "tfc: " << cyl_ptr->getTfc().x() << " "
                                 << cyl_ptr->getTfc().y() << " "
                                 << cyl_ptr->getTfc().z() << std::endl;
            std::cout << "bfc: " << cyl_ptr->getBfc().x() << " "
                                 << cyl_ptr->getBfc().y() << " "
                                 << cyl_ptr->getBfc().z() << std::endl;*/
            if (cyl_ptr->crossesBox(cubeSize)) {
                cyl_ptr->translate(-small_dx, -small_dy, -small_dz);
                cyl_ptr->rotateAroundZ(-small_gamma);
                cyl_ptr->rotateAroundY(-small_beta);
                cyl_ptr->rotateAroundX(-small_alpha);
                flag_transformation_done = false;
                //std::cout << "box cross: " << j << std::endl;
                continue;
            }
            for (uint other_j = 0; other_j < cyls_size; ++other_j) {
                if (other_j == j)
                    continue;
                //std::cout << "checking cross of pcs " << j << " " << other_j << std::endl;
                if (cyl_ptr->crossesOtherPolygonalCylinder(*polCyls[other_j], 0)) {
                    cyl_ptr->translate(-small_dx, -small_dy, -small_dz);
                    cyl_ptr->rotateAroundZ(-small_gamma);
                    cyl_ptr->rotateAroundY(-small_beta);
                    cyl_ptr->rotateAroundX(-small_alpha);
                    //std::cout << "crossing:-------------------------------- " << j << " " << other_j << std::endl;
                    flag_transformation_done = false;
                    break;
                }
            }
            if (flag_transformation_done) {
                shells[j]->rotateAroundX(small_alpha);
                shells[j]->rotateAroundY(small_beta);
                shells[j]->rotateAroundZ(small_gamma);
                shells[j]->translate(small_dx, small_dy, small_dz);
            }
        }
        std::string fname("geofiles/");
        fname += std::to_string(i) + ".geo";
        std::shared_ptr<CSGPrinterPolygons> printer_ptr;
        printer_ptr->printToCSGAsPolygonalCylindersShells(fname, polCyls, shells);
    }
    std::cout << "Finished mixing\n";


//    std::shared_ptr<CSGPrinterCircles> printer_ptr;
//    printer_ptr->printToCSGAsCircleCylindersShells(FNAME, polCyls, shells);
    std::shared_ptr<CSGPrinterCircles> printer_ptr;
    printer_ptr->printToCSGAsCircleCylindersShells(FNAME, polCyls, shells);
//    std::shared_ptr<CSGPrinterPolygons> printer_ptr;
//    printer_ptr->printToCSGAsPolygonalCylindersShells(FNAME, polCyls, shells);
    PercolationChecker pc(shells); 
    pc.sideToSide();
    std::cout << "--polygonal_end--\n";
    return 0;
}
