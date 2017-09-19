#include <vector>

#include "checkPercolation.hpp"


bool checkPercolation(std::vector<PolygonalCylinderInTheShell> pcitss) {
//bool checkPercolation(std::vector<PolygonalCylinder> pcitss) {
    std::vector<std::vector<int> > neighboring;
    for (int i = 0; i < pcitss.size(); ++i) {
        PolygonalCylinder pcits = pcitss[i];
        for (int j = 0; j < pcitss.size(); ++j) {
            if (i == j)
                continue;
            PolygonalCylinder otherPcits = pcitss[j];
            if (pcits.crossesOtherPolygonalCylinder(otherPcits, 1))
                std::cout << "cross\n";
        }
    }
    return false;
};
