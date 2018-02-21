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

    // simulating mixing
    std::cout << "Start mixing\n";
    uint cyls_size = polCyls.size();
    int mixing_step_attempt = 0;
    for (uint i = 0; i < ms; ++i) {
        //std::cout << "Mixing step: " << i << std::endl;
        std::vector<std::shared_ptr<PolygonalCylinder> > newCyls = polCyls;
        std::vector<std::shared_ptr<PolygonalCylinder> > newShells = shells;
        std::cout << i << " " << mixing_step_attempt << std::endl;
        for (uint pci = 0; pci < cyls_size; ++pci) {
            bool flag_append_ready = true;
            bool flag_appended = false;
            mixing_step_attempt = 0;
            while (!flag_appended && ++mixing_step_attempt < 10) {
                //std::cout << "Mixing step: " << ++mixing_step_attempt << std::endl;
                float small_alpha = PI_F * (static_cast<float>(rand()) / RAND_MAX) / 100;
                float small_beta = PI_F * (static_cast<float>(rand()) / RAND_MAX) / 100;
                float small_gamma = PI_F * (static_cast<float>(rand()) / RAND_MAX) / 100;
                float small_dx = static_cast<float>(rand() / RAND_MAX - 0.5) * cubeSize / 10;
                float small_dy = static_cast<float>(rand() / RAND_MAX - 0.5) * cubeSize / 10;
                float small_dz = static_cast<float>(rand() / RAND_MAX - 0.5) * cubeSize / 10;
                /*std::cout << small_alpha << " "
                          << small_beta << " "
                          << small_gamma << " "
                          << small_dx << " " << small_dy << " " << small_dz << std::endl;*/
                polCyls[pci]->rotateAroundX(PI_F * small_alpha);
                polCyls[pci]->rotateAroundY(PI_F * small_beta);
                polCyls[pci]->rotateAroundZ(PI_F * small_gamma);
                polCyls[pci]->translate(small_dx, small_dy, small_dz);
                shells[pci]->rotateAroundX(PI_F * small_alpha);
                shells[pci]->rotateAroundY(PI_F * small_beta);
                shells[pci]->rotateAroundZ(PI_F * small_gamma);
                shells[pci]->translate(small_dx, small_dy, small_dz);
                for (uint pcj = 0; pcj < cyls_size; ++pcj) {
                    if (pci == pcj)
                        continue;
                    if (polCyls[pci]->crossesBox(cubeSize)) {
                        flag_append_ready = false;
                        break;
                    }
                    if (polCyls[pci]->crossesOtherPolygonalCylinder(*polCyls[pcj], 0)) {
                        flag_append_ready = false;
                        break;
                    }
                }
                if (flag_append_ready) {
                    flag_appended = true;
                    newCyls[pci] = polCyls[pci];
                    newShells[pci] = shells[pci];
                }
                else {
                    polCyls[pci]->translate(-small_dx, -small_dy, -small_dz);
                    polCyls[pci]->rotateAroundZ(-PI_F * small_gamma);
                    polCyls[pci]->rotateAroundY(-PI_F * small_beta);
                    polCyls[pci]->rotateAroundX(-PI_F * small_alpha);
                    shells[pci]->translate(-small_dx, -small_dy, -small_dz);
                    shells[pci]->rotateAroundZ(-PI_F * small_gamma);
                    shells[pci]->rotateAroundY(-PI_F * small_beta);
                    shells[pci]->rotateAroundX(-PI_F * small_alpha);
                }
            }
        }
        polCyls = newCyls;
        shells = newShells;
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
