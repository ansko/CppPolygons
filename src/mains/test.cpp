#include <iostream>
#include <memory>
#include <vector>

#include "../polygonal_cylinder.hpp"
#include "../settings_parser.hpp"

#include "../printToCSGCircles.cpp" // this is a very bad practice!


//const float  PI_F = 3.14159265358979f; // this is a very bad practice!


int main(int argc, char **argv)
{
/*
    Test function used to test everything
  
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

    std::cout << "This is a test main function\n";

    // parsing settings
    SettingsParser sp("options.ini");
    sp.parseSettings();
    float cubeSize = (float)std::stod(sp.getProperty("CUBE_EDGE_LENGTH"));
    int n = (int)std::stod(sp.getProperty("VERTICES_NUMBER"));
    float h = (float)std::stod(sp.getProperty("THICKNESS"));
    float R = (float)std::stod(sp.getProperty("OUTER_RADIUS"));
    int N = (int)std::stod(sp.getProperty("DISKS_NUM"));
    int MAX_ATTEMPTS = (int)std::stod(sp.getProperty("MAX_ATTEMPTS"));
    std::string FNAME = sp.getProperty("FNAME");

    float r = R / 2; // for triangles. this is a very bad practice!

    std::cout << "MAX_ATTEMPTS = " << MAX_ATTEMPTS << std::endl
              << "number of filler particles = " << N << std::endl
              << "inner radius = " << r << std::endl
              << "thickness = " << h << std::endl
              << "cube edge length = " << cubeSize << std::endl
              << std::endl;

    // starting to create initial configuration
    std::vector<std::shared_ptr<PolygonalCylinder> > polCyls;
    int attempt = 0;
    int step = 0;
    while(polCyls.size() < N && ++attempt < MAX_ATTEMPTS) {
        ++step;
        if(step % (int(MAX_ATTEMPTS / 10)) == 0 || step == 1)
            std::cout << "step = " << step
                      << " of " << MAX_ATTEMPTS
                      << " ready " << polCyls.size()
                      << " of " << N
                      << std::endl;
        std::shared_ptr<PolygonalCylinder> polCyl_ptr =
            std::make_shared<PolygonalCylinder>(n, h, R);
        float dx = static_cast<float>(rand()) / RAND_MAX * cubeSize;
        float dy = static_cast<float>(rand()) / RAND_MAX * cubeSize;
        float dz = static_cast<float>(rand()) / RAND_MAX * cubeSize;
        polCyl_ptr->rotateAroundX(PI_F * static_cast<float>(rand()) / RAND_MAX);
        polCyl_ptr->rotateAroundY(PI_F * static_cast<float>(rand()) / RAND_MAX);
        polCyl_ptr->rotateAroundZ(PI_F * static_cast<float>(rand()) / RAND_MAX);
        polCyl_ptr->translate(dx, dy, dz);
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
    } 
    std::cout << std::endl;

    float pcVolume =  PI_F * pow(r, 2) * h;
    float cubeVolume = pow(cubeSize, 3);
    std::cout << "volume fraction = "
              << polCyls.size() * pcVolume / cubeVolume
              << std::endl;
    printToCSGAsCircleCylinders(FNAME, polCyls);

    std::cout << "Successful completion\n";

    return 0;
}
