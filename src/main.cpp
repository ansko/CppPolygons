#include <iostream>
#include <vector>

#include "point.hpp"
#include "polygon.hpp"
#include "polygonal_cylinder.hpp"
#include "vector.hpp"

#include "settings_parser.hpp"

int main(int argc, char **argv) {
    /*
    Point pt0(0, 0, 0), pt1(0, 1, 0), pt2(1, 1, 0), pt3(1, 0, 0), pt(0.9, 0.9, 0);
    std::vector<Point> pts;
    pts.push_back(pt0);
    pts.push_back(pt1);
    pts.push_back(pt2);
    pts.push_back(pt3);
    Polygon poly(pts);
    std::cout << poly.containsPoint(pt) << std::endl;
    */

    SettingsParser sp("options.ini");
    sp.parseSettings();

    return 0;
}
