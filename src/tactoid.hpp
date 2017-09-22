#pragma once

#include <vector>
#include <memory>

#include "settings_parser.hpp"
#include "polygonal_cylinder.hpp"


class Tactoid {
public:
    Tactoid(int stackNumber, float interlayerSpace, float layerThickness);
    int getStackNumber();
    std::vector<PolygonalCylinder> getPcs(int i);
    void translate(float dx, float dy, float dz);
    void rotateAroundX(float angle);
    void rotateAroundY(float angle);
    void rotateAroundZ(float angle);
    bool crossesOtherTac(Tactoid otherTac);
    bool crossesBox(float cubeEdgeLength);
private:
    int __stackNumber;
    float __interlayerSpace;
    float __layerThickness;
    std::vector <PolygonalCylinder> __pcs;
};
