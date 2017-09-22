#include "tactoid.hpp"
#include "settings_parser.hpp"


Tactoid::Tactoid(int stackNumber, float interlayerSpace, float layerThickness) {
    if(stackNumber % 2 == 0) {
        std::cout << "Incorrect number of layers in stack\n";
        return;
    }

    SettingsParser sp("options.ini");
    sp.parseSettings();
    int VERTICES_NUMBER = (int)std::stod(sp.getProperty("VERTICES_NUMBER"));
    float THICKNESS = (float)std::stod(sp.getProperty("THICKNESS"));
    float SHELL_THICKNESS = (float)std::stod(sp.getProperty("SHELL_THICKNESS"));
    float OUTER_RADIUS = (float)std::stod(sp.getProperty("OUTER_RADIUS"));

    __stackNumber = stackNumber;
    __interlayerSpace = interlayerSpace;
    __layerThickness = layerThickness;
    while(__pcs.size() < stackNumber) {
        for (auto& pc : __pcs) { 
            pc.translate(0, 0, __interlayerSpace + __layerThickness);
        }
        PolygonalCylinder pc(VERTICES_NUMBER,
                             THICKNESS,
                             OUTER_RADIUS);
        __pcs.push_back(pc);
    }
};

int Tactoid::getStackNumber() {
    return __stackNumber;
};


std::vector<PolygonalCylinder> Tactoid::getPcs(int i) {
    return __pcs;
}

void Tactoid::translate(float dx, float dy, float dz) {
    for (auto& pc : __pcs)
        pc.translate(dx, dy, dz);
    return;
}

void Tactoid::rotateAroundX(float angle) {
    for (auto& pc : __pcs)
        pc.rotateAroundX(angle);
    return;
}

void Tactoid::rotateAroundY(float angle) {
    for (auto& pc : __pcs)
        pc.rotateAroundY(angle);
    return;
}

void Tactoid::rotateAroundZ(float angle) {
    for (auto& pc : __pcs)
        pc.rotateAroundZ(angle);
    return;
}


bool Tactoid::crossesOtherTac(Tactoid otherTac) {
    //for (auto& pc : this->getPcs(0))
    //    for (auto& otherPc : otherTac.getPcs(0)) {
    //        if (pc.crossesOtherPolygonalCylinder(otherPc, 0))
    //            return true;
    //    }

// coarse implementation, should use big displacements in blending
    SettingsParser sp("options.ini");
    sp.parseSettings();
    int STACK_NUMBER = (int)std::stod(sp.getProperty("STACK_NUMBER"));
    STACK_NUMBER = STACK_NUMBER / 2;
    auto pc = this->getPcs(0)[STACK_NUMBER];
    auto otherPc = otherTac.getPcs(0)[STACK_NUMBER];
    return pc.crossesOtherPolygonalCylinder(otherPc, 0);
//
    return false;
}

bool Tactoid::crossesBox(float cubeEdgeLength) {
    for (auto& pc : this->getPcs(0))
        if (pc.crossesBox(cubeEdgeLength))
            return true;
    return false;
}
