#include <cmath>
#include <vector>

#include "det.hpp"

extern float determinant(std::vector<std::vector<float> > M, int dim) {
    int i = 0, j = 0, k = 0;
    float det = 0;
/*
    for (int i = 0; i < dim; i++)
    {
        for(int j=0; j < dim; j++)
            std::cout << M[i][j] << " ";
        std::cout << std::endl;
    }
*/
    if (dim == 1)
        det = M[0][0];
    else if (dim == 2)
        det = M[0][0] * M[1][1] - M[1][0] * M[0][1];
    else {
        for(j = 0; j < dim; ++j) {
            std::vector<std::vector<float> > newM;
            newM = M;
            newM.erase(newM.begin());
            for (i = 0; i < dim - 1; i++) {
                newM[i].erase(newM[i].begin() + j);
            }
            det += pow(-1.0, 1.0 + j + 1.0) * M[0][j] * determinant(newM, dim - 1);
        }
    }
    return det;
}
