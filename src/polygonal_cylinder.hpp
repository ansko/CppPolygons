#pragma once

#include <memory>
#include <vector>

#include "point.hpp"
#include "polygon.hpp"


class PolygonalCylinder {
public:
    PolygonalCylinder(int verticesNumber, float thickness, float outerRadius);
    Polygon topFacet();
    Polygon bottomFacet();
    std::vector<Polygon> facets();
    bool crossesOtherPolygonalCylinder(PolygonalCylinder otherPolygonalCylinder);
    bool crossesBox(float boxSize);
    void translate(float dx, float dy, float dz);
    void rotateAroundX(float angle);
    void rotateAroundY(float angle);
    void rotateAroundZ(float angle);
private:
    std::vector<Polygon> __facets;
    std::shared_ptr<Polygon> topFacet_ptr, bottomFacet_ptr;
};
