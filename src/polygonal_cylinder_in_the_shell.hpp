#pragma once

#include "polygonal_cylinder.hpp"


class PolygonalCylinderInTheShell : public PolygonalCylinder {
public:
    PolygonalCylinderInTheShell(
        int verticesNumber,
        float thickness,
        float outerRadius,
        float shellThickness
    );
    //PolygonalCylinderInTheShell(PolygonalCylinder pc);
    bool crossesBox(float boxSize) override;
protected:
    float __shellThickness;
};
