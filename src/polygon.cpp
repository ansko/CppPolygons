#include <iostream>
#include <vector>

#include "polygon.hpp"

Polygon::Polygon(std::vector<Point> vertices) {
    __vertices = vertices;
};

std::vector<Point> Polygon::vertices() {
    return __vertices;
};

Point Polygon::center() {
    Vector center;
    for (auto vertex : __vertices)
        center = center + Vector(vertex.x(), vertex.y(), vertex.z());
    center = center / __vertices.size();
    return Point(center.x(), center.y(), center.z());
}

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
    if (flagSelfCrossesOther == true)
            return true;
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
    if (flagOtherCrossesSelf == true)
        return true;
    return false;
};

bool Polygon::crossesOtherPolygon2(Polygon otherPolygon) {
    return true;
}

bool Polygon::containsPoint(Point pt) {
    int s = __vertices.size(), flag=0;
    Vector center, otherCenter;
    for (auto vertex : __vertices)
        center = center + Vector(vertex.x(), vertex.y(), vertex.z());
    center = center / s;
    Point pt0, pt1, pt2;
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
    return false;
}; 

// this implementation is made for percolation case---------------------|
// when cell is multiplied 27 times                                   //|
/*bool Polygon::crossesBox(float boxSize) {                           //|
    for (int i = 0; i < __vertices.size(); ++i) {                     //|
        auto vertex = __vertices[i];                                  //|
        auto vertex2 = Point();                                       //|
        if (i == 0)                                                   //|
            vertex2 = __vertices[__vertices.size() - 1];              //|
        else                                                          //|
            vertex2 = __vertices[i - 1];                              //|
        if (vertex.x() < 0 && vertex2.x() > 0)                        //|
            if(vertex.y() > 0 && vertex.y() < boxSize &&              //|
               vertex.z() > 0 && vertex.z() < boxSize)                //|
                return true;                                          //|
        if (vertex.y() < 0 && vertex2.y() > 0)                        //|
            if(vertex.x() > 0 && vertex.x() < boxSize &&              //|
               vertex.z() > 0 && vertex.z() < boxSize)                //|
                return true;                                          //|
        if (vertex.z() < 0 && vertex2.z() > 0)                        //|
            if(vertex.x() > 0 && vertex.x() < boxSize &&              //|
               vertex.y() > 0 && vertex.y() < boxSize)                //|
                return true;                                          //|
        if (vertex.x() > boxSize && vertex2.x() < boxSize)            //|
            if(vertex.y() > 0 && vertex.y() < boxSize &&              //|
               vertex.z() > 0 && vertex.z() < boxSize)                //|
                return true;                                          //|
        if (vertex.y() > boxSize && vertex2.y() < boxSize)            //|
            if(vertex.x() > 0 && vertex.x() < boxSize &&              //|
               vertex.z() > 0 && vertex.z() < boxSize)                //|
                return true;                                          //|
        if (vertex.z() > boxSize && vertex2.z() < boxSize)            //|
            if(vertex.x() > 0 && vertex.x() < boxSize &&              //|
               vertex.y() > 0 && vertex.y() < boxSize)                //|
                return true;                                          //|
    }                                                                 //|
    return false;                                                     //|
};*/                                                                  //|
                                                                      //|
                                                                      //|
// this implementation is made for all other cases //-------------------|
bool Polygon::crossesBox(float boxSize) {
    for (int i = 0; i < __vertices.size(); ++i) {
        auto vertex = __vertices[i];
        if (vertex.x() < 0 || vertex.x() > boxSize)
            return true;
        if (vertex.y() < 0 || vertex.y() > boxSize)
            return true;
        if (vertex.z() < 0 || vertex.z() > boxSize)
            return true;
    }
    return false;
};
