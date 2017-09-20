#include <algorithm>
#include <set>
#include <vector>

#include "checkPercolation.hpp"


bool checkPercolation(std::vector<PolygonalCylinderInTheShell> pcitss) {
    std::cout << "finding indices of crossings\n";
    std::vector<std::vector<int> > crossings;
    for (int i = 0; i < pcitss.size(); ++i) {
        PolygonalCylinderInTheShell pcits = pcitss[i];
        for(int j = i + 1; j < pcitss.size(); ++j) {
            PolygonalCylinderInTheShell otherPcits = pcitss[j];
            if (pcits.crossesOtherPolygonalCylinder(otherPcits, 1)) {
                std::cout << i << " " << j << std::endl;
                std::vector<int> crossing;
                crossing.push_back(crossings.size());
                crossing.push_back(i);
                crossing.push_back(j);
                crossings.push_back(crossing);
            }
        }
    }
    std::cout << "----\n";

    std::cout << "outputing found crossings\n";
    for(auto crossing : crossings)
        std::cout << crossing[0] << " "
                  << crossing[1] << " " 
                  << crossing[2] << std::endl;

    std::set<int> stoPop;
    for (int i = 0; i < crossings.size(); ++i)
        for (int j = i + 1; j < crossings.size(); ++j) {
            if (crossings[i][1] == crossings[j][1] &&
                crossings[i][2] == crossings[j][2])
                stoPop.insert(j);
        }
    std::vector<int> vtoPop;
    for (auto element : stoPop) 
        vtoPop.push_back(element);
    std::sort(vtoPop.begin(), vtoPop.end());
    std::reverse(vtoPop.begin(), vtoPop.end());
    std::cout << "\nvShould remove these crossings:\n---\n";
    for (int i = 0; i < vtoPop.size(); ++i) {
        std::cout << vtoPop[i] << " ";
        crossings.erase(crossings.begin() + i);
    }

    std::cout << "\n----------\n";

    std::cout << "crossings after pop\n";
    for(auto crossing : crossings)
        std::cout << crossing[0] << " "
                  << crossing[1] << " " 
                  << crossing[2] << std::endl;

    std::cout << "\n----------\n";

    vtoPop.clear();
    std::vector<std::vector<int> > chains;
    for (auto crossing : crossings) {
        int flag = 0;
        for (auto& chain : chains) {
            if (std::find(chain.begin(),
                          chain.end(),
                          crossing[1]) != chain.end()) {
                flag = 1;
                chain.push_back(crossing[2]);
            }
            if (std::find(chain.begin(),
                          chain.end(),
                          crossing[2]) != chain.end()) {
                flag = 2;
                chain.push_back(crossing[1]);
            }
        }
        if (flag == 0) {
             std::vector<int> newChain;
             newChain.push_back(crossing[1]);
             newChain.push_back(crossing[2]);
             chains.push_back(newChain);
        }
    }

    std::cout << "now chains contain pcitss.getNumber()\n";
    for (auto chain : chains) {
        std::cout << "chain before removing the same numbers\n";
        for (auto el : chain)
            //std::cout << el << " " << pcitss[el].getNumber() << std::endl;
            std::cout << el << " ";
        std::cout << std::endl;
        int startSize = chain.size();
        std::set<int> schain(chain.begin(), chain.end());
        int endSize = schain.size();
        chain.clear();
        for (auto el : schain)
            chain.push_back(pcitss[el].getNumber());
        std::sort(chain.begin(), chain.end());
        std::cout << "----\n";
        std::cout << "chain after removing the same numbers\n";
        for (auto el : chain)
            std::cout << el << " ";
        std::cout << std::endl;
        if (startSize == endSize)
            std::cout << "no percolation in this chain\n";
        else {
            std::cout << "there is percolation in this chain\n";
            return true;
        }
    }
    std::cout << "end of checkpercolation\n";

    

    return false;
};

