#include "sparse_matrix.hpp"
#include <iostream>
#include <cmath>

using namespace std;

int main() {
    SparseMatrix m1(5, 5);
    SparseMatrix m2(4, 3);
    SparseMatrix mult = m1 * m2;
    return 0;
}

