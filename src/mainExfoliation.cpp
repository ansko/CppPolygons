#include <cstdlib>
#include <ctime>

#include <iostream>
#include <vector>

#include "point.hpp"
#include "polygon.hpp"
#include "polygonal_cylinder.hpp"
#include "vector.hpp"

#include "printToCSG.cpp"

#define CUBE_EDGE_LENGTH 20.
#define MAX_ATTEMPTS 50000.
#define DISKS_NUM 10.
#define VERTICES_NUMBER 8.
#define THICKNESS 1.
#define OUTER_RADIUS 5.
#define FNAME "1.geo"

const float  PI_F = 3.14159265358979f;


int main(int argc, char **argv) {
    std::cout << "Start!\n";
    std::vector<PolygonalCylinder> pcs;
    int attempt = 0;

    PolygonalCylinder pc(VERTICES_NUMBER, THICKNESS, OUTER_RADIUS);

    std::srand(unsigned(std::time(0)));
    while (pcs.size() < DISKS_NUM && attempt < MAX_ATTEMPTS) {
        attempt++;
        std::cout << attempt << " " << pcs.size() << " ";
        PolygonalCylinder pc(VERTICES_NUMBER, THICKNESS, OUTER_RADIUS);
        float dx = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        float dy = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        float dz = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        std::cout << " " << dx << " " << dy << " " << dz << " ";
        pc.translate(dx, dy, dz);
        pc.rotateAroundX(PI_F * static_cast<float>(rand()) / RAND_MAX);
        pc.rotateAroundY(PI_F * static_cast<float>(rand()) / RAND_MAX);
        pc.rotateAroundZ(PI_F * static_cast<float>(rand()) / RAND_MAX);
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
            std::cout << "appended\n";
        }
        else if (flag == 1)
            std::cout << "disk cross\n";
        else
            std::cout << "box cross\n";
    }
    std::cout << "volume fraction = " << pcs.size() * PI_F * pow(OUTER_RADIUS, 2) * THICKNESS / pow(CUBE_EDGE_LENGTH, 3) << std::endl;
    printToCSG(FNAME, pcs);

    return 0;
}
