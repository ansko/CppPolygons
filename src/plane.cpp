#include "plane.hpp"

Plane::Plane(float a, float b, float c, float d) {
    __a = a;
    __b = b;
    __c = c;
    __d = d;
};

Plane::Plane(Vector n, Point pt) {
    __n = n;
    __pt0 = pt;
    __a = n.x();
    __b = n.y();
    __c = n.z();
    __d = -n.x() * pt.x() - n.y() * pt.y() - n.z() * pt.z();
};

Plane::Plane(Point pt0, Point pt1, Point pt2) {
    __pt0 = pt0;
    __pt1 = pt1;
    __pt2 = pt2;
    __a = (pt1.y() - pt0.y()) * (pt2.z() - pt0.z());
    __a -= (pt1.z() - pt0.z()) * (pt2.y() - pt0.y());
    __b = -(pt1.x() - pt0.x()) * (pt2.z() - pt0.z());
    __b += (pt2.x() - pt0.x()) * (pt1.z() - pt0.z());
    __c = (pt1.x() - pt0.x()) * (pt2.y() - pt0.y());
    __c -= (pt2.x() - pt0.x()) * (pt1.y() - pt0.y());
    __d = -pt0.x() * (pt1.y() - pt0.y()) * (pt2.z() - pt0.z());
    __d += pt0.x() * (pt1.z() - pt0.z()) * (pt2.y() - pt0.y());
    __d += pt0.y() * (pt1.x() - pt0.x()) * (pt2.z() - pt0.z());
    __d -= pt0.y() * (pt2.x() - pt0.x()) * (pt1.z() - pt0.z());
    __d -= pt0.z() * (pt1.x() - pt0.x()) * (pt2.y() - pt0.y());
    __d += pt0.z() * (pt2.x() - pt0.x()) * (pt1.y() - pt0.y());
};


float Plane::a() {
    return __a;
};

float Plane::b() {
    return __b;
};

float Plane::c() {
    return __c;
};

float Plane::d() {
    return __d;
};

Vector Plane::n() {
    return Vector(__a, __b, __c);
};

Point Plane::pt0() {
    return __pt0;
};

Point Plane::pt1() {
    return __pt1;
};

Point Plane::pt2() {
    return __pt2;
};

Point Plane::ptCross() {
    return __ptCross;
};


bool Plane::isCrossedByLineSegment(LineSegment ls) {
    Point pt0 = __pt0;
    Point pt1 = __pt1;
    Point pt2 = __pt2;
    Point pt3 = ls.begin();
    Point pt4 = ls.end();
    Vector v01 = Vector(pt0, pt1);
    Vector v02 = Vector(pt0, pt2);
    Vector v03 = Vector(pt0, pt3);
    Vector v04 = Vector(pt0, pt4);
    std::vector<std::vector<float> > M1, M2;
    std::vector<float> s;
    M1.push_back(s);
    M1.push_back(s);
    M1.push_back(s);
    M1[0].push_back(v01.x());
    M1[0].push_back(v01.y());
    M1[0].push_back(v01.z());
    M1[1].push_back(v02.x());
    M1[1].push_back(v02.y());
    M1[1].push_back(v02.z());
    M1[2].push_back(v03.x());
    M1[2].push_back(v03.y());
    M1[2].push_back(v03.z());
    M2.push_back(s);
    M2.push_back(s);
    M2.push_back(s);
    M2[0].push_back(v01.x());
    M2[0].push_back(v01.y());
    M2[0].push_back(v01.z());
    M2[1].push_back(v02.x());
    M2[1].push_back(v02.y());
    M2[1].push_back(v02.z());
    M2[2].push_back(v04.x());
    M2[2].push_back(v04.y());
    M2[2].push_back(v04.z());

    float det1 = determinant(M1, 3);
    float det2 = determinant(M2, 3);
    if (det1 * det2 < 0) {
        Vector v3 = Vector(pt3.x(), pt3.y(), pt3.z());
        Vector v34 = Vector(pt3, pt4);
        Vector vCross = v3 + v34 * abs(det1) / (abs(det1) + abs(det2));
        __ptCross = Point(vCross.x(), vCross.y(), vCross.z());
        //std::cout << "cross1 " << __ptCross.x() << " " << __ptCross.y() << " " << __ptCross.z() << std::endl;
        return true;
    }
    else if (det1 == 0) {
        __ptCross = pt3;
        //std::cout << "cross2 " << __ptCross.x() << " " << __ptCross.y() << " " << __ptCross.z() << std::endl;
        return true;
    }
    else if (det2 == 0) {
        __ptCross = pt4;
        //std::cout << "cross1 " << __ptCross.x() << " " << __ptCross.y() << " " << __ptCross.z() << std::endl;
        return true;
    }
    return false;
};
