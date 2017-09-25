#include <cstdlib>
#include <ctime>

#include <iostream>
#include <memory>
#include <vector>

#include "point.hpp"
#include "polygon.hpp"
#include "polygonal_cylinder.hpp"
#include "vector.hpp"

#include "printToCSG2.cpp"
#include "settings_parser.hpp"

//const float  PI_F = 3.14159265358979f;


int main(int argc, char **argv) {
    std::cout << "Start!\n";
//    std::vector<PolygonalCylinder> pcs;
    std::vector<std::shared_ptr<PolygonalCylinder> > pc_ptrs;
    int attempt = 0;

    std::shared_ptr<SettingsParser> sp_ptr = std::make_shared<SettingsParser>("options.ini");
//    SettingsParser sp("options.ini");
//    sp.parseSettings();
    sp_ptr->parseSettings();
//    float CUBE_EDGE_LENGTH = (float)std::stod(sp.getProperty("CUBE_EDGE_LENGTH"));
    float CUBE_EDGE_LENGTH = (float)std::stod(sp_ptr->getProperty("CUBE_EDGE_LENGTH"));
//    int VERTICES_NUMBER = (int)std::stod(sp.getProperty("VERTICES_NUMBER"));
    int VERTICES_NUMBER = (int)std::stod(sp_ptr->getProperty("VERTICES_NUMBER"));
//    float THICKNESS = (float)std::stod(sp.getProperty("THICKNESS"));
    float THICKNESS = (float)std::stod(sp_ptr->getProperty("THICKNESS"));
//    float OUTER_RADIUS = (float)std::stod(sp.getProperty("OUTER_RADIUS"));
    float OUTER_RADIUS = (float)std::stod(sp_ptr->getProperty("OUTER_RADIUS"));
//    int DISKS_NUM = (int)std::stod(sp.getProperty("DISKS_NUM"));
    int DISKS_NUM = (int)std::stod(sp_ptr->getProperty("DISKS_NUM"));
//    int MAX_ATTEMPTS = (int)std::stod(sp.getProperty("MAX_ATTEMPTS"));
    int MAX_ATTEMPTS = (int)std::stod(sp_ptr->getProperty("MAX_ATTEMPTS"));
//    std::string FNAME = sp.getProperty("FNAME");
    std::string FNAME = sp_ptr->getProperty("FNAME");

    std::srand(unsigned(std::time(0)));
//    while (pcs.size() < DISKS_NUM && attempt < MAX_ATTEMPTS) {
    while (pc_ptrs.size() < DISKS_NUM && attempt < MAX_ATTEMPTS) {
        attempt++;
        if (attempt % 1000 == 0)
//            std::cout << attempt << " " << pcs.size() << " \n";
            std::cout << attempt << " " << pc_ptrs.size() << " \n";
        std::shared_ptr<PolygonalCylinder> pc_ptr = std::make_shared<PolygonalCylinder>(VERTICES_NUMBER,
                                                                                        THICKNESS,
                                                                                        OUTER_RADIUS);
        float dx = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        float dy = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        float dz = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        //pc.rotateAroundX(PI_F * static_cast<float>(rand()) / RAND_MAX);
        //pc.rotateAroundY(PI_F * static_cast<float>(rand()) / RAND_MAX);
        //pc.rotateAroundZ(PI_F * static_cast<float>(rand()) / RAND_MAX);
        //pc.translate(dx, dy, dz);
        pc_ptr->rotateAroundX(PI_F * static_cast<float>(rand()) / RAND_MAX);
        pc_ptr->rotateAroundY(PI_F * static_cast<float>(rand()) / RAND_MAX);
        pc_ptr->rotateAroundZ(PI_F * static_cast<float>(rand()) / RAND_MAX);
        pc_ptr->translate(dx, dy, dz);

        int flag = 0;
        //if (pc.crossesBox(CUBE_EDGE_LENGTH))
        if (pc_ptr->crossesBox(CUBE_EDGE_LENGTH))
            flag = 2;
        else
            //for(auto& oldPc : pcs)
            for(auto oldPc_ptr : pc_ptrs)
//                if (pc.crossesOtherPolygonalCylinder(oldPc)) {
                if (pc_ptr->crossesOtherPolygonalCylinder(*oldPc_ptr, 0)) {
                    flag = 1;
                    break;
                }
        if (flag == 0) {
            pc_ptrs.push_back(pc_ptr);
        }
    }
    float pcVolume =  PI_F * pow(OUTER_RADIUS, 2) * THICKNESS;
    float cubeVolume = pow(CUBE_EDGE_LENGTH, 3);
    std::cout << "volume fraction = "
              << pc_ptrs.size() * pcVolume / cubeVolume
              << std::endl;
    printToCSG2(FNAME, pc_ptrs);

    return 0;
}
