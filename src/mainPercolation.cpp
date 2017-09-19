#include <ctime>
#include <iostream>

#include "point.hpp"
#include "polygon.hpp"
#include "polygonal_cylinder.hpp"
#include "polygonal_cylinder_in_the_shell.hpp"
#include "vector.hpp"
#include "printToCSG.cpp"
#include "settings_parser.hpp"

#include "checkPercolation.hpp"

#define PI_F 3.14159265358979


int main(int argc, char **argv) {
    std::cout << "Start!\n";
    std::vector<PolygonalCylinderInTheShell> pcitss;
    //std::vector<PolygonalCylinder> pcitss;
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
    float SHELL_THICKNESS = (float)std::stod(sp.getProperty("SHELL_THICKNESS"));

    int currentNumber = 0;
    int additionalDisksNum = 0;

    std::srand(unsigned(std::time(0)));
    while (pcitss.size() - additionalDisksNum < DISKS_NUM && attempt < MAX_ATTEMPTS) {
        attempt++;
        if (attempt % 1 == 0) {
            std::cout << attempt << " " << pcitss.size() << " \n";
            bool flagPercolation = checkPercolation(pcitss);
            std::cout << flagPercolation << std::endl;
            if (flagPercolation == true)
                return 0;
        }
        PolygonalCylinderInTheShell pcits(VERTICES_NUMBER,
                                          THICKNESS,
                                          OUTER_RADIUS,
                                          SHELL_THICKNESS);
        //PolygonalCylinder pcits(VERTICES_NUMBER, THICKNESS, OUTER_RADIUS);
        float dx = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        float dy = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        float dz = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        pcits.rotateAroundX(PI_F * static_cast<float>(rand()) / RAND_MAX);
        pcits.rotateAroundY(PI_F * static_cast<float>(rand()) / RAND_MAX);
        pcits.rotateAroundZ(PI_F * static_cast<float>(rand()) / RAND_MAX);
        pcits.translate(dx, dy, dz);
        int flag = 0;
        for(auto& oldPcits : pcitss)
            if (pcits.crossesOtherPolygonalCylinder(oldPcits, 0)) {
                flag = 1;
                break;
            }
        if (flag == 0) {
            currentNumber++;
            for(int indexX = -1; indexX < 2; ++indexX)
                for(int indexY = -1; indexY < 2; ++indexY)
                    for(int indexZ = -1; indexZ < 2; ++indexZ) {
                        PolygonalCylinderInTheShell pcits1 = pcits;
                        //PolygonalCylinder pcits1 = pcits;
                        pcits1.translate(CUBE_EDGE_LENGTH * indexX,
                                         CUBE_EDGE_LENGTH * indexY,
                                         CUBE_EDGE_LENGTH * indexZ);
                        if ((0 < pcits1.topFacet().center().x()  &&
                             CUBE_EDGE_LENGTH > pcits1.topFacet().center().x() &&
                             0 < pcits1.topFacet().center().y()  &&
                             CUBE_EDGE_LENGTH > pcits1.topFacet().center().y() &&
                             0 < pcits1.topFacet().center().z()  &&
                             CUBE_EDGE_LENGTH > pcits1.topFacet().center().z()) ||
                             pcits1.crossesBox(CUBE_EDGE_LENGTH)) {
                                 pcits1.setNumber(currentNumber);
                                 if (indexX != 0 && indexY != 0 && indexZ != 0)
                                     ++additionalDisksNum;
                                 pcitss.push_back(pcits1);
                             }
                    }
        }
    }
    float disksVolume = pcitss.size() * PI_F * pow(OUTER_RADIUS, 2) * THICKNESS;
    std::cout << "volume fraction = "
              << disksVolume / pow(CUBE_EDGE_LENGTH, 3)
              << std::endl;
    printToCSG(FNAME, pcitss, 0);

    return 0;
}
