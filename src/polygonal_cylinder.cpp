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
        centralVertices.push_back(Point(outerRadius * cos(centralAngle * i),\
                                        outerRadius * sin(centralAngle * i),\
                                        thickness / 2));
        centralVertices.push_back(Point(outerRadius * cos(centralAngle * i),\
                                        outerRadius * sin(centralAngle * i),\
                                        -thickness / 2));
        centralVertices.push_back(Point(outerRadius * cos(centralAngle * (i - 1)),\
                                        outerRadius * sin(centralAngle * (i - 1)),\
                                        thickness / 2));
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
    std::vector<Polygon> polygons, otherPolygons;
    polygons.push_back(*topFacet_ptr);
    polygons.push_back(*bottomFacet_ptr);
    for (auto facet : __facets)
        polygons.push_back(facet);
    otherPolygons.push_back(otherPolygonalCylinder.topFacet());
    otherPolygons.push_back(otherPolygonalCylinder.bottomFacet());
    for (auto facet : otherPolygonalCylinder.facets())
        polygons.push_back(facet);
    for(int i = 0; i < polygons.size(); ++i)
        for(int j = 0; j < otherPolygons.size(); ++j)
            if (polygons[i].crossesOtherPolygon(otherPolygons[j]))
                return true;
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
