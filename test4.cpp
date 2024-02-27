#include "sparse_matrix.hpp"
#include <iostream>

int main() {
    SparseMatrix m1(5, 5);
    double v = m1[5][5];
    std::cout << v << std::endl;
    return 0;
}

