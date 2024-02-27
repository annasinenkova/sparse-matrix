#ifndef SPARSEMATRIX_HPP
#define SPARSEMATRIX_HPP

#include <iostream>

class SparseMatrix {
    static double eps;
    struct Sp_matr_elem {
        double value;
        size_t index;
        Sp_matr_elem *next;
    };
    struct Sp_matr_str {
        size_t index;
        Sp_matr_str *next;
        Sp_matr_elem *first;
    };
    size_t n, m;
    Sp_matr_str *matrix;
    
public:
    SparseMatrix(int np, int mp) : n(np), m(mp), matrix(NULL) {};
    SparseMatrix(const SparseMatrix & sp_matr);
    friend std::ostream & operator << (std::ostream & out, const SparseMatrix & c);
    double get(size_t index1, size_t index2) const;
    void set(size_t index1, size_t index2, double value);
    void add(size_t index1, size_t index2, double value);
    size_t num_rows() const;
    size_t num_columns() const;
    bool operator ==(const SparseMatrix & sp_matr) const;
    bool operator !=(const SparseMatrix & sp_matr) const;
    SparseMatrix operator +(const SparseMatrix & sp_matr) const;
    SparseMatrix operator *(const SparseMatrix & sp_matr) const;
    SparseMatrix & operator =(const SparseMatrix & sp_matr);
    void set_eps(double e);
    ~SparseMatrix();

    class Indexed_element;
    class Offset_indexed_element;
    class Intermediate_object;
    class Offset_intermediate_object;

    class Indexed_element {
        friend class Offset_indexed_element;
        friend class Intermediate_object;
        int index1, index2;
        SparseMatrix *matrix_ptr;
        Indexed_element(SparseMatrix *mp, int ip1, int ip2) : index1(ip1), index2(ip2), matrix_ptr(mp) {};
    public:
        operator double();
        double operator =(double c);
        double operator +=(double c);
        double operator -=(double c);
        double operator *=(double c);
        double operator /=(double c);
    };

    class Offset_indexed_element {
        friend class Intermediate_object;
        int index1, index2;
        SparseMatrix *matrix_ptr;
        const SparseMatrix *const_matrix_ptr;
        Offset_indexed_element(SparseMatrix *mp, int ip1, int ip2) : index1(ip1), index2(ip2), matrix_ptr(mp) {};
        Offset_indexed_element(const SparseMatrix *mp, int ip1, int ip2) : index1(ip1), index2(ip2), const_matrix_ptr(mp) {};
    public:
        Indexed_element operator *();
        double operator *() const;
    };

    class Intermediate_object {
        friend class Offset_intermediate_object;
        friend class SparseMatrix;
        int index;
        SparseMatrix *matrix_ptr;
        const SparseMatrix *const_matrix_ptr;
        Intermediate_object(SparseMatrix *mp, int i) : index(i), matrix_ptr(mp) {};
        Intermediate_object(const SparseMatrix *mp, int i) : index(i), const_matrix_ptr(mp) {};
    public:
        Indexed_element operator [](int i);
        Indexed_element operator *();
        double operator [](int i) const;
        double operator *() const;
        Offset_indexed_element operator +(int c);
        const Offset_indexed_element operator +(int c) const;
    };

    class Offset_intermediate_object {
        friend class SparseMatrix;
        int index;
        SparseMatrix *matrix_ptr;
        const SparseMatrix *const_matrix_ptr;
        Offset_intermediate_object(SparseMatrix *mp, int i) : index(i), matrix_ptr(mp) {};
        Offset_intermediate_object(const SparseMatrix *mp, int i) : index(i), const_matrix_ptr(mp) {};
    public:
        Intermediate_object operator *();
        const Intermediate_object operator *() const;
    };

    Intermediate_object operator [](int index);
    const Intermediate_object operator [](int index) const;
    Intermediate_object operator *();
    const Intermediate_object operator *() const;
    Offset_intermediate_object operator +(int c) ;
    const Offset_intermediate_object operator +(int c) const;
};

#endif
