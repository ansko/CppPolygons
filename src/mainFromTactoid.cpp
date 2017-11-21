#include <cmath>
#include <ctime>
#include <memory>

#include "polygonal_cylinder.hpp"
#include "tactoid.hpp"
#include "settings_parser.hpp"

#include "printToCSGCircles.cpp"

//float PI_F = 3.14159265358979


int main(int argc, char **argv) {

    SettingsParser sp("options.ini");
    sp.parseSettings();
    float CUBE_EDGE_LENGTH = (float)std::stod(sp.getProperty("CUBE_EDGE_LENGTH"));
    float VERTICES_NUMBER = (float)std::stod(sp.getProperty("VERTICES_NUMBER"));
    float THICKNESS = (float)std::stod(sp.getProperty("THICKNESS"));
    float OUTER_RADIUS = (float)std::stod(sp.getProperty("OUTER_RADIUS"));
    float INTERLAYER_SPACE = (float)std::stod(sp.getProperty("INTERLAYER_SPACE"));
    float SHELL_THICKNESS = (float)std::stod(sp.getProperty("SHELL_THICKNESS"));
    int STACK_NUMBER = (int)std::stod(sp.getProperty("STACK_NUMBER"));
    int DISKS_NUM = (int)std::stod(sp.getProperty("DISKS_NUM"));
    int MAX_ATTEMPTS = (int)std::stod(sp.getProperty("MAX_ATTEMPTS"));
    int STEPS_NUM = (int)std::stod(sp.getProperty("STEPS_NUM"));
    std::string FNAME = sp.getProperty("FNAME");

    float verticesNumber = VERTICES_NUMBER;
    float edgeLength = OUTER_RADIUS * 2  * sin(2 * PI_F / verticesNumber);
    float innerRadius = edgeLength / 2 / tan(PI_F / verticesNumber);

    // Generating initial state
    int attempt = 0;
    std::srand(unsigned(std::time(0)));
    std::vector <Tactoid> tacs;
    while (tacs.size() < int(DISKS_NUM / STACK_NUMBER) && attempt < MAX_ATTEMPTS) {
        ++attempt;
        if (attempt % 10 == 0)
            std::cout << "Attempt " 
                      << attempt 
                      << " done " 
                      << tacs.size()
                      << " of "
                      << int(DISKS_NUM / STACK_NUMBER)
                      << std::endl;
        Tactoid tac = Tactoid(STACK_NUMBER,
                              INTERLAYER_SPACE,
                              THICKNESS);
        float dx = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        float dy = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        float dz = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        float angle = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        tac.rotateAroundX(angle);
        angle = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        tac.rotateAroundY(angle);
        angle = static_cast<float>(rand()) / RAND_MAX * CUBE_EDGE_LENGTH;
        tac.rotateAroundZ(angle);
        tac.translate(dx, dy, dz);
        int flagTacTacIntersection = 0;
        int flagTacBoxIntersection = 0;
        for (auto& oldTac : tacs)
            if(tac.crossesOtherTac(oldTac))
                flagTacTacIntersection = 1;
        if (tac.crossesBox(CUBE_EDGE_LENGTH))
            flagTacBoxIntersection = 1;
        if (flagTacTacIntersection == 0 && flagTacBoxIntersection == 0)
            tacs.push_back(tac);
    }
    float circle = PI_F * innerRadius * innerRadius;
    float tacVolume = STACK_NUMBER * THICKNESS * circle;
                      //(STACK_NUMBER * THICKNESS + 
                      //(STACK_NUMBER - 1) * INTERLAYER_SPACE +
                      //2 * SHELL_THICKNESS) * circle;
    float tacsVolume = tacs.size() * tacVolume;
    float boxVolume = pow(CUBE_EDGE_LENGTH, 3);
    std::cout << "Tactoid system generated with volume fraction "
              << tacsVolume / boxVolume
              << std::endl;

    std::vector<PolygonalCylinder> pcs;
    for (auto& tac : tacs) {
        std::shared_ptr<std::vector<PolygonalCylinder> > pcsTmp_ptr = 
            std::make_shared<std::vector<PolygonalCylinder> >(tac.getPcs(0));
        pcs.reserve(pcs.size() + pcsTmp_ptr->size());
        pcs.insert(pcs.end(), pcsTmp_ptr->begin(), pcsTmp_ptr->end());
    }
    if (pcs.size() != 0)
        printToCSGAsCircleCylinders("Start.geo", pcs);
    else
        return 0;

    // simulation of blending
    float coeffDist = 0.1;
    float coeffAngle = 10.;
    float smallDistance = coeffDist * CUBE_EDGE_LENGTH;
    float smallAngleX = coeffAngle * 2 * PI_F;
    float smallAngleY = coeffAngle * 2 * PI_F;
    float smallAngleZ = coeffAngle * 2 * PI_F;
    for (int i = 1; i <= STEPS_NUM; ++i) {
        std::cout << "Blending, step " << i << std::endl;
        for (int j = 0; j < pcs.size(); j++) {
            PolygonalCylinder pc = pcs[j];
            float dx = static_cast<float>(2 * rand() - RAND_MAX) / RAND_MAX * smallDistance;
            float dy = static_cast<float>(2 * rand() - RAND_MAX) / RAND_MAX * smallDistance;
            float dz = static_cast<float>(2 * rand() - RAND_MAX) / RAND_MAX * smallDistance;
            float angle = (2 * rand() - RAND_MAX) / RAND_MAX * smallAngleX;
            pc.rotateAroundX(0.01);
            angle = (2 * rand() - RAND_MAX) / RAND_MAX * smallAngleY;
            pc.rotateAroundY(0.01);
            angle = (2 * rand() - RAND_MAX) / RAND_MAX * smallAngleZ;
            pc.rotateAroundZ(0.01);
            pc.translate(dx, dy, dz);
            int flagPcPcIntersection = 0;
            int flagPcBoxIntersection = 0;
            for (auto k = 0; k < pcs.size(); ++k) {
                if (k == j)
                    continue; 
                PolygonalCylinder otherPc = pcs[k];
                if (pc.crossesOtherPolygonalCylinder(otherPc, 0))
                    flagPcPcIntersection = 1;
            }
            if (pc.crossesBox(CUBE_EDGE_LENGTH))
                flagPcBoxIntersection = 1;
            if (flagPcPcIntersection == 0 && flagPcBoxIntersection == 0)
                pcs[j] = pc;
        }
        //if (i % std::min((int)(STEPS_NUM / 10), int(coeffDist / 0.01)) == 0) {
        if (1) {
            std::string stringNum = std::to_string(i);
            stringNum = stringNum + ".geo";
            printToCSGAsCircleCylinders(stringNum, pcs);
        }
    }
    return 0;
}
