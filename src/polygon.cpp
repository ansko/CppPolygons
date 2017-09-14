#include <iostream>
#include <vector>

#include "polygon.hpp"


Polygon::Polygon(std::vector<Point> vertices) {
    __vertices = vertices;
};

std::vector<Point> Polygon::vertices() {
    return __vertices;
};

bool Polygon::crossesOtherPolygon(Polygon otherPolygon) {
    std::vector<Point> otherVertices = otherPolygon.vertices();
    Plane otherPlane = Plane(otherVertices[0], otherVertices[1], otherVertices[2]);
    Plane plane = Plane(__vertices[0], __vertices[1], __vertices[2]);
    bool flagSelfCrossesOther = false;
    bool flagOtherCrossesSelf = false;
    for (int i = 0; i < __vertices.size(); ++i) {
        Point ptBegin = __vertices[i];
        Point ptEnd;
        if (i != __vertices.size() - 1)
            ptEnd = __vertices[i + 1];
        else
            ptEnd = __vertices[0];
        LineSegment ls = LineSegment(ptBegin, ptEnd);
        if (otherPlane.isCrossedByLineSegment(ls)) {
            Point ptCross = otherPlane.ptCross();
            if (otherPolygon.containsPoint(ptCross)) {
                flagSelfCrossesOther = true;
                break;
            }
        }
    }
    if (flagSelfCrossesOther == false)
            return false;
    for (int i = 0; i < otherVertices.size(); ++i) {
        Point ptBegin = otherVertices[i];
        Point ptEnd;
        if (i != otherVertices.size() - 1)
            ptEnd = otherVertices[i + 1];
        else
            ptEnd = otherVertices[0];
        LineSegment ls = LineSegment(ptBegin, ptEnd);
        if (plane.isCrossedByLineSegment(ls)) {
            Point ptCross = plane.ptCross();
            if (this->containsPoint(ptCross)) {
                flagOtherCrossesSelf = true;
                break;
            }
        }
    }
    if (flagOtherCrossesSelf == false)
        return false;
    return true;
};

bool Polygon::containsPoint(Point pt) {
    int s = __vertices.size(), j;
    float r;
    /*Point pt0, pt1, pt2;
    for(int i = 0; i < s; ++i) {
        if (i == s - 1) {
            pt0 = __vertices[i];
            pt1 = __vertices[0];
            pt2 = __vertices[1];
        }
        else if (i == s - 2) {
            pt0 = __vertices[i];
            pt1 = __vertices[i + 1];
            pt2 = __vertices[0];
        }
        else {
            pt0 = __vertices[i];
            pt1 = __vertices[i + 1];
            pt2 = __vertices[i + 2];
        }
        Plane pl = Plane(pt0, pt1, pt2);
        Vector v20 = Vector(pt2, pt0);
        Vector v21 = Vector(pt2, pt1);
        Vector v2pt = Vector(pt2, pt);
        if (v20.vectorMultiply(v21).scalarMultiply(v20.vectorMultiply(v2pt)) < 0)
            return false;
    }*/
    if (s % 2 == 0)
        j = s / 2;
    else
        j = (s + 1) / 2;
    r = Vector(__vertices[0], __vertices[j]).length();
    int flag = 1;
    for(int i = 0; i < s; i++) {
        if (r < Vector(vertices[i], pt).length())
    }
    return true;
}; 

bool Polygon::crossesBox(float boxSize) {
    for (int i = 0; i < __vertices.size(); ++i) {
        auto vertex = __vertices[i];
        if (vertex.x() < 0 || vertex.y() < 0 || vertex.z() < 0)
            return true;
        if (vertex.x() > boxSize || vertex.y() > boxSize || vertex.z() > boxSize)
            return true;
    }
    return false;
};
