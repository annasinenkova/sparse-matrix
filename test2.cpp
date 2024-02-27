#include "sparse_matrix.hpp"
#include <iostream>

int main() {
    SparseMatrix m1(5, 5);
    m1[0][0] = 2;
    m1[0][1] = 1;
    m1[1][0] = 4;
    m1[1][1] = 5;
    const SparseMatrix m2 = m1;
    const SparseMatrix m3 = m1;
    SparseMatrix sum = m2 + m3;
    SparseMatrix mult = m2 * m3;
    std::cout << (m2 != m3) << std::endl;
    std::cout << **m2 - *(*m3 + 1) << std::endl;
    return 0;
}

