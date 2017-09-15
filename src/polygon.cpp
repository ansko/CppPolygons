#include <iostream>
#include <vector>

#include "polygon.hpp"

#define CUBE_EDGE_LENGTH 20.
#define MAX_ATTEMPTS 50000.
#define DISKS_NUM 100.
#define VERTICES_NUMBER 8.
#define THICKNESS 1.
#define OUTER_RADIUS 5.
#define FNAME "1.geo"

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
            //std::cout << "cross1 " << ptCross.x() << " " << ptCross.y() << " " << ptCross.z() << std::endl;
            //if (otherPolygon.containsPoint(ptCross)) {
                flagSelfCrossesOther = true;
                break;
            //}
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
            //std::cout << "cross2 " << ptCross.x() << " " << ptCross.y() << " " << ptCross.z() << std::endl;
            //if (this->containsPoint(ptCross)) {
                flagOtherCrossesSelf = true;
                break;
            //}
        }
    }
    if (flagOtherCrossesSelf == false)
        return false;
    //std::cout << "polygons cross\n";
    return true;
};

bool Polygon::containsPoint(Point pt) {
    int s = __vertices.size(), flag=0;
    Vector center, otherCenter;
    for (auto vertex : __vertices)
        center = center + Vector(vertex.x(), vertex.y(), vertex.z());
    center = center / s;
    float l = Vector(Point(center.x(), center.y(), center.z()), pt).length();
    //std::cout << l << std::endl;
    if (l < OUTER_RADIUS)
        return true;
    else
        return false;
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
        Vector v02 = Vector(pt0, pt2);
        Vector v01 = Vector(pt0, pt1);
        Vector v0pt = Vector(pt0, pt);
        float tmp = v02.vectorMultiply(v01).scalarMultiply(v01.vectorMultiply(v0pt));
        if (tmp > 0)
            flag = 1;
    }
    if (flag == 0)
        return true;
    return false;*/
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
