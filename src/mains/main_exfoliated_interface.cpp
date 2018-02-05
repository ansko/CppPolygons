#include <ctime>

#include <iostream>
#include <memory>
#include <vector>

#include "../geometries/polygonal_cylinder.hpp"
#include "../settings_parser.hpp"
#include "../CSGPrinterCircles.hpp"


int main(int argc, char **argv)
{
/*
    This is the simplest main function that creates a system of 
    exfoliated particles without the interface, 
    randonly ditributed in the cubic cell.

  
    Nomenclature:
        cubeSize - the length of the cube edge
        n - vertices number in polygon into what the resulting top/bottom
            of the cylinder will be inscribed
        h - thickness of the cylinder
        R - radius of the circumcircle about the polygon
        N - number of exfoliated particles
        MAX_ATTEMPTS - maximum number of the attempts to generate initial
                       configuration
        FNAME - output geofile name


    Usage:
        ./exe_name
*/
    std::cout << time(0) << std::endl;
    // parsing settings
    SettingsParser sp("options.ini");
    sp.parseSettings();
    float cubeSize = (float)std::stod(sp.getProperty("CUBE_EDGE_LENGTH"));
    int n = (int)std::stod(sp.getProperty("VERTICES_NUMBER"));
    float h = (float)std::stod(sp.getProperty("THICKNESS"));
    float sh = (float)std::stod(sp.getProperty("SHELL_THICKNESS"));
    float R = (float)std::stod(sp.getProperty("OUTER_RADIUS"));
    int N = (int)std::stod(sp.getProperty("DISKS_NUM"));
    int MAX_ATTEMPTS = (int)std::stod(sp.getProperty("MAX_ATTEMPTS"));
    std::string FNAME = sp.getProperty("FNAME");
    float edgeLength = R * 2  * sin(PI_F / n);
    float r = R * cos(PI_F / n);
    std::cout << "AR = " << 2 * r /h << std::endl
              //<< "MAX_ATTEMPTS = " << MAX_ATTEMPTS << std::endl
              << "number of filler particles = " << N << std::endl
              << "inner radius = " << r << std::endl
              << "thickness = " << h << std::endl
              << "shell thickness = " << sh << std::endl
              << "cube edge length = " << cubeSize << std::endl;

    // starting to create initial configuration
    std::vector<std::shared_ptr<PolygonalCylinder> > polCyls;
    std::vector<std::shared_ptr<PolygonalCylinder> > shells;
    int attempt = 0;
    while(polCyls.size() < N && ++attempt < MAX_ATTEMPTS) {
        std::shared_ptr<PolygonalCylinder> polCyl_ptr =
            std::make_shared<PolygonalCylinder>(n, h, R);
        std::shared_ptr<PolygonalCylinder> sh_ptr =
            std::make_shared<PolygonalCylinder>(n, h + sh, R);
        float dx = static_cast<float>(rand()) / RAND_MAX * cubeSize;
        float dy = static_cast<float>(rand()) / RAND_MAX * cubeSize;
        float dz = static_cast<float>(rand()) / RAND_MAX * cubeSize;
        float alpha = static_cast<float>(rand()) / RAND_MAX;
        float beta = static_cast<float>(rand()) / RAND_MAX;
        float gamma = static_cast<float>(rand()) / RAND_MAX;
        polCyl_ptr->rotateAroundX(PI_F * alpha);
        polCyl_ptr->rotateAroundY(PI_F * beta);
        polCyl_ptr->rotateAroundZ(PI_F * gamma);
        polCyl_ptr->translate(dx, dy, dz);
        sh_ptr->rotateAroundX(PI_F * alpha);
        sh_ptr->rotateAroundY(PI_F * beta);
        sh_ptr->rotateAroundZ(PI_F * gamma);
        sh_ptr->translate(dx, dy, dz);
        if (polCyl_ptr->crossesBox(cubeSize))
            continue;
        int flag = 0;
        for(auto oldPc : polCyls)
            if (polCyl_ptr->crossesOtherPolygonalCylinder(*oldPc, 0)) {
                flag = 1;
                break;
            }
        if(flag == 1)
            continue;
        polCyls.push_back(polCyl_ptr);
        shells.push_back(sh_ptr);
    } 
    std::cout << "made " << polCyls.size() << std::endl;

    float pcVolume =  PI_F * pow(r, 2) * h;
    float shVolume =  PI_F * pow(r + sh, 2) * (h + 2 * sh);
    float cubeVolume = pow(cubeSize, 3);
    std::cout << "volume fraction of filler = "
              << polCyls.size() * pcVolume / cubeVolume
              << "\nvolume fraction of interface = "
              << polCyls.size() * (shVolume - pcVolume) / cubeVolume
              << "\nvolume fraction of not-matrix = "
              << polCyls.size() * shVolume / cubeVolume
              << std::endl;
    CSGPrinterCircles printer;
    printer.printToCSGAsCircleCylindersShells(FNAME, polCyls, shells);

    return 0;
}
