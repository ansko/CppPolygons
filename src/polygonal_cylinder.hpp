#pragma once

#include <memory>
#include <string>
#include <vector>

#include "point.hpp"
#include "polygon.hpp"
#include "settings_parser.hpp"


class PolygonalCylinder {
public:
    PolygonalCylinder(int verticesNumber, float thickness, float outerRadius);
    Polygon topFacet();
    Polygon bottomFacet();
    std::vector<Polygon> facets();
    bool crossesOtherPolygonalCylinder(PolygonalCylinder otherPolygonalCylinder, int mode);
    virtual bool crossesBox(float boxSize);
    void translate(float dx, float dy, float dz);
    void rotateAroundX(float angle);
    void rotateAroundY(float angle);
    void rotateAroundZ(float angle);
    void setName(std::string nameToSet);
    void setNumber(int numberToSet);
    int getNumber();
protected:
    std::vector<Polygon> __facets;
    std::shared_ptr<Polygon> topFacet_ptr, bottomFacet_ptr;
    std::string name;
    int number;
    float __outerRadius;
};
