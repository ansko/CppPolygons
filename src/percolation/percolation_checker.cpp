#include "percolation_checker.hpp"


PercolationChecker::PercolationChecker(
    std::vector<std::shared_ptr<PolygonalCylinder> > shell_ptrs)
{
    this->shell_ptrs = shell_ptrs;
}


bool PercolationChecker::sideToSide()
{
    bool flag = false;
    std::ofstream fout;
    fout.open("intersections.log");
    std::vector<std::pair<int, int> > allIntersections;
    std::vector<std::vector<int> > chains;

    for(uint i = 0; i < shell_ptrs.size(); ++i)
        for(uint j = 0; j < shell_ptrs.size(); ++j) {
            if(i == j)
                continue;
            PolygonalCylinder pc = *shell_ptrs[i];
            PolygonalCylinder pc1 = *shell_ptrs[j];
            if(pc.crossesOtherPolygonalCylinder(pc1, 0)) {
                fout << i << " " << j << std::endl;
                allIntersections.push_back(std::pair<int, int>(i, j));
            }
        }


    SettingsParser sp("options.ini");
    sp.parseSettings();
    float cubeSize = (float)std::stod(sp.getProperty("CUBE_EDGE_LENGTH"));
    fout.close();
    fout.open("coords.log");
    for(uint i = 0; i < shell_ptrs.size(); ++i) {
        auto shell_ptr = shell_ptrs[i];
        float minx = cubeSize;
        float maxx = 0;
        float miny = cubeSize;
        float maxy = 0;
        float minz = cubeSize;
        float maxz = 0;
        for (auto poly : shell_ptr->facets())
            for (auto vertex : poly.vertices()) {
                if (vertex.x() < minx)
                    minx = vertex.x();
                else if (vertex.x() > maxx)
                    maxx = vertex.x();
                if (vertex.y() < miny)
                    miny = vertex.y();
                else if (vertex.y() > maxy)
                    maxy = vertex.y();
                if (vertex.z() < minz)
                    minz = vertex.z();
                else if (vertex.z() > maxz)
                    maxz = vertex.z();
            }
        fout << minx << " " << maxx << " "
             << miny << " " << maxy << " "
             << minz << " " << maxz << std::endl;
    }

/*
    for (uint l = 0; l < allIntersections.size(); ++l) {
        for (uint i = 0; i < allIntersections.size(); ++i) {
            bool ifChainExists = false;
            for (uint k = 0; k < chains.size(); ++k) {
                bool ifFirstIsSeen = false;
                bool ifSecondIsSeen = false;
                std::vector<int> chain = chains[k];
                for (uint j = 0; j < chain.size(); ++j) {
                    if (chain[j] == allIntersections[i].first)
                        ifFirstIsSeen = true;
                    if (chain[j] == allIntersections[i].second)
                        ifSecondIsSeen = true;
                }
                if (ifFirstIsSeen && ifSecondIsSeen) {
                    ifChainExists = true;
                    continue;
                }
                if (ifFirstIsSeen)
                    chain.push_back(allIntersections[i].second);
                else if (ifSecondIsSeen)
                    chain.push_back(allIntersections[i].first);
                ifChainExists = true;
            }
            if (!ifChainExists) {
                std::vector<int> chain;
                chain.push_back(allIntersections[i].first);
                chain.push_back(allIntersections[i].second);
                chains.push_back(chain);
            }
        }
    }


  // output of chains and percolation flags
    SettingsParser sp("options.ini");
    sp.parseSettings();
    float cubeSize = (float)std::stod(sp.getProperty("CUBE_EDGE_LENGTH"));
    for(uint i = 0; i < chains.size(); ++i) {
        bool crossTop = false;
        bool crossBottom = false;
        bool crossLeft = false;
        bool crossRight = false;
        bool crossFace = false;
        bool crossBack = false;
        for(uint j = 0; j < chains[i].size(); ++j) {
            std::cout << chains[i][j] << " ";
            std::vector<Polygon> polys = shell_ptrs[chains[i][j]]->facets();
            for (auto poly : polys) {
                std::vector<Point> pts = poly.vertices();
                for (auto pt : pts) {
                    if (pt.x() > cubeSize)
                        crossRight = true;
                    else if (pt.x() < 0) 
                        crossLeft = true;
                    if (pt.y() > cubeSize)
                        crossBack = true;
                    else if (pt.y() < 0)
                        crossFace = true;
                    if (pt.z() > cubeSize)
                        crossTop = true;
                    else if (pt.z() < 0)
                        crossBottom = true;
                }
            }
        }
        if ((crossRight && crossLeft) ||
            (crossBack && crossFace) ||
            (crossTop && crossBottom)) {
                std::cout << "percolation!";
                flag = true;
        }
        std::cout << std::endl;

    }
*/
    return flag;
}
