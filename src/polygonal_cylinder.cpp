#include <cmath>
#include <iostream>

#include "polygonal_cylinder.hpp"

const float  PI_F = 3.14159265358979f;


PolygonalCylinder::PolygonalCylinder(int verticesNumber, float thickness, float outerRadius) {
    float centralAngle = 2 * PI_F / verticesNumber;
    std::vector<Point> centralVertices, topVertices, bottomVertices;
    for(int i = 0; i < verticesNumber; i++) {
        centralVertices.clear();
        topVertices.push_back(Point(outerRadius * cos(centralAngle * i),\
                                    outerRadius * sin(centralAngle * i),\
                                    thickness / 2));
        bottomVertices.push_back(Point(outerRadius * cos(centralAngle * i),\
                                       outerRadius * sin(centralAngle * i),\
                                       -thickness / 2));
        centralVertices.push_back(Point(outerRadius * cos(centralAngle * (i - 1)),\
                                        outerRadius * sin(centralAngle * (i - 1)),\
                                        thickness / 2));
        centralVertices.push_back(Point(outerRadius * cos(centralAngle * i),\
                                        outerRadius * sin(centralAngle * i),\
                                        thickness / 2));
        centralVertices.push_back(Point(outerRadius * cos(centralAngle * i),\
                                        outerRadius * sin(centralAngle * i),\
                                        -thickness / 2));
        centralVertices.push_back(Point(outerRadius * cos(centralAngle * (i - 1)),\
                                        outerRadius * sin(centralAngle * (i - 1)),\
                                        -thickness / 2));
        __facets.push_back(Polygon(centralVertices));
    }
    topFacet_ptr = std::make_shared<Polygon>(Polygon(topVertices));
    bottomFacet_ptr = std::make_shared<Polygon>(Polygon(bottomVertices));
};

std::vector<Polygon> PolygonalCylinder::facets() {
    return __facets;
};

Polygon PolygonalCylinder::topFacet() {
    return *topFacet_ptr;
};

Polygon PolygonalCylinder::bottomFacet() {
    return *bottomFacet_ptr;
};

bool PolygonalCylinder::crossesOtherPolygonalCylinder(PolygonalCylinder otherPolygonalCylinder) {
    SettingsParser sp("options.ini");
    sp.parseSettings();
    float THICKNESS = (float)std::stod(sp.getProperty("THICKNESS"));
    float OUTER_RADIUS = (float)std::stod(sp.getProperty("OUTER_RADIUS"));

    //std::cout << "are very close or very far? ";
    // if polygonal cylinders are very close or very far
    Point tc = topFacet_ptr->center();
    Point bc = bottomFacet_ptr->center();
    Point otherTc = otherPolygonalCylinder.topFacet().center();
    Point otherBc = otherPolygonalCylinder.bottomFacet().center();
    Vector vc = Vector(tc.x() + bc.x(), tc.y() + bc.y(), tc.z() + bc.z()) / 2;
    Point c(vc.x(), vc.y(), vc.z());
    Vector otherVc = Vector(otherTc.x() + otherBc.x(),\
                            otherTc.y() + otherBc.y(),\
                            otherTc.z() + otherBc.z()) / 2;
    Point otherC(otherVc.x(), otherVc.y(), otherVc.z());
    float centersDistance = Vector(c, otherC).length();
    float veryCloseDistance = std::min(THICKNESS, 2 * OUTER_RADIUS);
    float veryFarDistance = 2 * pow(pow(OUTER_RADIUS, 2) + pow(THICKNESS/2, 2), 0.5);
    if (centersDistance > veryFarDistance) {
        //std::cout << "surely far\n";
        return false;
    }
    else if (centersDistance < veryCloseDistance) {
        //std::cout << "surely close\n";
        return true;
    }
    else {
        //std::cout << "don't know\n";
    }


    // if main axes of the cylinders lie on far lines
    // http://en.wikipedia.org/wiki/Skew_lines#Distance_between_two_skew_lines
    // x = x1 + td1 and x = x2 + td2
    // d = |(d1 x d2) / |d1 x d2| * (x1 - x2)|
    Point x1 = bottomFacet_ptr->center();
    Vector d1(bottomFacet_ptr->center(), topFacet_ptr->center());
    Point x2 = otherPolygonalCylinder.bottomFacet().center();
    Vector d2(otherPolygonalCylinder.bottomFacet().center(),\
              otherPolygonalCylinder.topFacet().center());
    Vector n = d1.vectorMultiply(d2) / d1.vectorMultiply(d2).length();
    Vector dx(x1, x2);
    float d = fabs(n.scalarMultiply(dx));
    if (d > 2 * OUTER_RADIUS)
        return false;

    // intermediate situation
    std::vector<Polygon> polys, otherPolys;
    polys.push_back(*topFacet_ptr);
    polys.push_back(*bottomFacet_ptr);
    for (auto facet : __facets)
        polys.push_back(facet);
    otherPolys.push_back(otherPolygonalCylinder.topFacet());
    otherPolys.push_back(otherPolygonalCylinder.bottomFacet());
    for (auto facet : otherPolygonalCylinder.facets())
        otherPolys.push_back(facet);
    for (auto poly : polys)
        for (auto otherPoly : otherPolys) {
            if (poly.crossesOtherPolygon(otherPoly))
                return true;
        }
    return false;
};

bool PolygonalCylinder::crossesBox(float boxSize) {
    std::vector<Polygon> polygons;
    polygons.push_back(*topFacet_ptr);
    polygons.push_back(*bottomFacet_ptr);

    for(int i = 0; i < 2; i++)
        if (polygons[i].crossesBox(boxSize))
            return true;
    return false;
};

void PolygonalCylinder::translate(float dx, float dy, float dz) {
    std::vector<std::vector<float> > M;
    std::vector<float> s;
    M.push_back(s);
    M.push_back(s);
    M.push_back(s);
    M.push_back(s);
    M[0].push_back(1);
    M[0].push_back(0);
    M[0].push_back(0);
    M[0].push_back(0);
    M[1].push_back(0);
    M[1].push_back(1);
    M[1].push_back(0);
    M[1].push_back(0);
    M[2].push_back(0);
    M[2].push_back(0);
    M[2].push_back(1);
    M[2].push_back(0);
    M[3].push_back(dx);
    M[3].push_back(dy);
    M[3].push_back(dz);
    M[3].push_back(1);
    std::vector<Polygon> polygons;
    polygons.push_back(*topFacet_ptr);
    polygons.push_back(*bottomFacet_ptr);
    for (auto& facet : __facets)
        polygons.push_back(facet);

    for (int i = 0; i < polygons.size(); ++i) {
        std::vector<Point> pts;
        for (auto& vertex : polygons[i].vertices()) {
            Vector v(vertex.x(), vertex.y(), vertex.z());
            v = v.dot4(M);
            pts.push_back(Point(v.x(), v.y(), v.z()));
        }
        polygons[i] = Polygon(pts);
    }
    topFacet_ptr = std::make_shared<Polygon>(polygons[0]);
    bottomFacet_ptr = std::make_shared<Polygon>(polygons[1]);
    __facets.clear();
    for (int i = 2; i < polygons.size(); ++i)
        __facets.push_back(polygons[i]);
};

void PolygonalCylinder::rotateAroundX(float angle) {
    std::vector<std::vector<float> > M;
    std::vector<float> s;
    M.push_back(s);
    M.push_back(s);
    M.push_back(s);
    M.push_back(s);
    M[0].push_back(1);
    M[0].push_back(0);
    M[0].push_back(0);
    M[0].push_back(0);
    M[1].push_back(0);
    M[1].push_back(cos(angle));
    M[1].push_back(-sin(angle));
    M[1].push_back(0);
    M[2].push_back(0);
    M[2].push_back(sin(angle));
    M[2].push_back(cos(angle));
    M[2].push_back(0);
    M[3].push_back(0);
    M[3].push_back(0);
    M[3].push_back(0);
    M[3].push_back(1);
    std::vector<Polygon> polygons;
    polygons.push_back(*topFacet_ptr);
    polygons.push_back(*bottomFacet_ptr);
    for (auto facet : __facets)
        polygons.push_back(facet);

    for (int i = 0; i < polygons.size(); ++i) {
        std::vector<Point> pts;
        for (auto vertex : polygons[i].vertices()) {
            Vector v(vertex.x(), vertex.y(), vertex.z());
            v = v.dot4(M);
            pts.push_back(Point(v.x(), v.y(), v.z()));
        }
        polygons[i] = Polygon(pts);
    }
    topFacet_ptr = std::make_shared<Polygon>(polygons[0]);
    bottomFacet_ptr = std::make_shared<Polygon>(polygons[1]);
    __facets.clear();
    for (int i = 2; i < polygons.size(); ++i)
        __facets.push_back(polygons[i]);
};

void PolygonalCylinder::rotateAroundY(float angle) {
    std::vector<std::vector<float> > M;
    std::vector<float> s;
    M.push_back(s);
    M.push_back(s);
    M.push_back(s);
    M.push_back(s);
    M[0].push_back(cos(angle));
    M[0].push_back(0);
    M[0].push_back(sin(angle));
    M[0].push_back(0);
    M[1].push_back(0);
    M[1].push_back(1);
    M[1].push_back(0);
    M[1].push_back(0);
    M[2].push_back(-sin(angle));
    M[2].push_back(0);
    M[2].push_back(cos(angle));
    M[2].push_back(0);
    M[3].push_back(0);
    M[3].push_back(0);
    M[3].push_back(0);
    M[3].push_back(1);
    std::vector<Polygon> polygons;
    polygons.push_back(*topFacet_ptr);
    polygons.push_back(*bottomFacet_ptr);
    for (auto facet : __facets)
        polygons.push_back(facet);

    for (int i = 0; i < polygons.size(); ++i) {
        std::vector<Point> pts;
        for (auto vertex : polygons[i].vertices()) {
            Vector v(vertex.x(), vertex.y(), vertex.z());
            v = v.dot4(M);
            pts.push_back(Point(v.x(), v.y(), v.z()));
        }
        polygons[i] = Polygon(pts);
    }
    topFacet_ptr = std::make_shared<Polygon>(polygons[0]);
    bottomFacet_ptr = std::make_shared<Polygon>(polygons[1]);
    __facets.clear();
    for (int i = 2; i < polygons.size(); ++i)
        __facets.push_back(polygons[i]);
};

void PolygonalCylinder::rotateAroundZ(float angle) {
    std::vector<std::vector<float> > M;
    std::vector<float> s;
    M.push_back(s);
    M.push_back(s);
    M.push_back(s);
    M.push_back(s);
    M[0].push_back(cos(angle));
    M[0].push_back(-sin(angle));
    M[0].push_back(0);
    M[0].push_back(0);
    M[1].push_back(sin(angle));
    M[1].push_back(cos(angle));
    M[1].push_back(0);
    M[1].push_back(0);
    M[2].push_back(0);
    M[2].push_back(0);
    M[2].push_back(1);
    M[2].push_back(0);
    M[3].push_back(0);
    M[3].push_back(0);
    M[3].push_back(0);
    M[3].push_back(1);
    std::vector<Polygon> polygons;
    polygons.push_back(*topFacet_ptr);
    polygons.push_back(*bottomFacet_ptr);
    for (auto facet : __facets)
        polygons.push_back(facet);

    for (int i = 0; i < polygons.size(); ++i) {
        std::vector<Point> pts;
        for (auto vertex : polygons[i].vertices()) {
            Vector v(vertex.x(), vertex.y(), vertex.z());
            v = v.dot4(M);
            pts.push_back(Point(v.x(), v.y(), v.z()));
        }
        polygons[i] = Polygon(pts);
    }
    topFacet_ptr = std::make_shared<Polygon>(polygons[0]);
    bottomFacet_ptr = std::make_shared<Polygon>(polygons[1]);
    __facets.clear();
    for (int i = 2; i < polygons.size(); ++i)
        __facets.push_back(polygons[i]);
};
