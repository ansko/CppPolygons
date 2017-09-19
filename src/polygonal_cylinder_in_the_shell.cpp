#include "polygonal_cylinder_in_the_shell.hpp"


PolygonalCylinderInTheShell::PolygonalCylinderInTheShell(
        int verticesNumber,
        float thickness,
        float outerRadius,
        float shellThickness
    ) : PolygonalCylinder(
        verticesNumber,
        thickness,
        outerRadius
    ) {
    __shellThickness = shellThickness;
};

/*PolygonalCylinderInTheShell::PolygonalCylinderInTheShell(PolygonalCylinder pc) {
   // __facets = pc.facets();
   // topFacet_ptr = pc.topFacet();
   // bottomFacet_ptr = pc.bottomFacet();
};*/

bool PolygonalCylinderInTheShell::crossesBox(float boxSize) {
    std::vector<Polygon> polygons;
    polygons.push_back(*topFacet_ptr);
    polygons.push_back(*bottomFacet_ptr);
    Point tc = this->topFacet().center();
    Point bc = this->bottomFacet().center();
    Vector vtc(tc.x(), tc.y(), tc.z());
    Vector vbc(bc.x(), bc.y(), bc.z());
    Vector vc = vtc / 2 + vbc / 2;
    Point c(vc.x(), vc.y(), vc.z());
    Vector vctc(c, tc);
    Vector vcbc(c, bc);
    for(int i = 0; i < 2; i++) {
        Polygon polygon = polygons[i];
        Vector vcpolyc(c, polygon.center());
        float l = vcpolyc.length();
        float coeff = (__shellThickness + l) / l - 1;
        Vector vtoadd = vcpolyc * coeff;
        std::vector<Point> newpolypts;
        for (auto pt : polygon.vertices()) {
            Vector vpt(pt.x(), pt.y(), pt.z());
            vpt = vpt + vtoadd;
            pt = Point(vpt.x(), vpt.y(), vpt.z());
            newpolypts.push_back(pt);
        }
        polygon = Polygon(newpolypts);
        if (polygon.crossesBox(boxSize))
            return true;
    }
    return false;
};
