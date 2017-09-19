#include <cstdlib>
#include <ctime>

#include <iostream>
#include <vector>

#include "point.hpp"
#include "polygon.hpp"
#include "polygonal_cylinder.hpp"
#include "vector.hpp"

#include "printToCSG.cpp"
#include "settings_parser.hpp"

const float  PI_F = 3.14159265358979f;


int main(int argc, char **argv) {
    std::cout << "Start!\n";
    std::vector<PolygonalCylinder> pcs;
    int attempt = 0;


    SettingsParser sp("options.ini");
    sp.parseSettings();
    float CUBE_EDGE_LENGTH = (float)std::stod(sp.getProperty("CUBE_EDGE_LENGTH"));
    int VERTICES_NUMBER = (int)std::stod(sp.getProperty("VERTICES_NUMBER"));
    float THICKNESS = (float)std::stod(sp.getProperty("THICKNESS"));
    float OUTER_RADIUS = (float)std::stod(sp.getProperty("OUTER_RADIUS"));
    int DISKS_NUM = (int)std::stod(sp.getProperty("DISKS_NUM"));
    int MAX_ATTEMPTS = (int)std::stod(sp.getProperty("MAX_ATTEMPTS"));
    std::string FNAME = sp.getProperty("FNAME");

    std::srand(unsigned(std::time(0)));
    while (pcs.size() < DISKS_NUM && attempt < MAX_ATTEMPTS) {
        attempt++;
        if (attempt % 1000 == 0)
            std::cout << attempt << " " << pcs.size() << " \n";
        PolygonalCylinder pc(VERTICES_NUMBER, THICKNESS, OUTER_RADIUS);
        float dx = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        float dy = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        float dz = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        pc.rotateAroundX(PI_F * static_cast<float>(rand()) / RAND_MAX);
        pc.rotateAroundY(PI_F * static_cast<float>(rand()) / RAND_MAX);
        pc.rotateAroundZ(PI_F * static_cast<float>(rand()) / RAND_MAX);
        pc.translate(dx, dy, dz);
        int flag = 0;
        if (pc.crossesBox(CUBE_EDGE_LENGTH))
            flag = 2;
        else
            for(auto& oldPc : pcs)
                if (pc.crossesOtherPolygonalCylinder(oldPc)) {
                    flag = 1;
                    break;
                }
        if (flag == 0) {
            pcs.push_back(pc);
        }
        else if (flag == 1)
        {}
        else
        {}
    }
    std::cout << "volume fraction = " << pcs.size() * PI_F * pow(OUTER_RADIUS, 2) * THICKNESS / pow(CUBE_EDGE_LENGTH, 3) << std::endl;
    printToCSG(FNAME, pcs);

    return 0;
}
