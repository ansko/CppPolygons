#include <cmath>
#include <vector>

#include "det.hpp"

//extern float determinant(std::vector<std::vector<float> > M, int dim) {
extern float determinant(std::array<std::array<float, 3>, 3> M, int dim) {
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
    else if (dim == 3)
        det = M[0][0] * (M[1][1] * M[2][2] - M[2][1] * M[1][2]) -
              M[0][1] * (M[1][0] * M[2][2] - M[1][2] * M[2][0]) +
              M[0][2] * (M[1][0] * M[2][1] - M[2][0] * M[1][1]);
    else if (dim == 4)
        det = M[0][0] * (M[1][1] * (M[2][2] * M[3][3] - M[3][2] * M[2][3]) -
                         M[1][2] * (M[2][1] * M[3][3] - M[3][1] * M[2][3]) +
                         M[1][3] * (M[2][1] * M[3][2] - M[3][1] * M[2][2])) -
              M[0][1] * (M[1][0] * (M[2][2] * M[3][3] - M[2][3] * M[3][2]) -
                         M[1][2] * (M[2][0] * M[3][3] - M[3][0] * M[2][3]) +
                         M[1][3] * (M[2][0] * M[3][2] - M[3][0] * M[2][2])) +
              M[0][2] * (M[1][0] * (M[2][1] * M[3][3] - M[2][3] * M[3][1]) -
                         M[1][1] * (M[2][0] * M[3][3] - M[3][0] * M[2][3]) +
                         M[1][3] * (M[2][0] * M[3][1] - M[2][1] * M[3][0])) -
              M[0][3] * (M[1][0] * (M[2][1] * M[3][2] - M[2][2] * M[3][1]) -
                         M[1][1] * (M[2][0] * M[3][2] - M[2][2] * M[3][0]) +
                         M[1][2] * (M[2][0] * M[3][1] - M[2][1] * M[3][0]));
    return det;
}
