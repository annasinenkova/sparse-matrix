#include "sparse_matrix.hpp"
#include <iostream>
#include <cstdlib>
#include <cmath>


std::ostream & operator << (std::ostream & out, const SparseMatrix & c) {
    for (size_t i = 0; i < c.n; ++i) {
        for (size_t j = 0; j < c.m; ++j) {
            std::cout << c.get(i, j) << ' ';
        }
        std::cout << std::endl;
    }
    return out;
}

double SparseMatrix::eps = 1e-7;

SparseMatrix::SparseMatrix(const SparseMatrix & sp_matr) {
    this->n = sp_matr.n;
    this->m = sp_matr.m;
    this->matrix = NULL;
    Sp_matr_str *str, *prev_str, *newstr;
    Sp_matr_elem *elem, *prev_elem, *newelem;      
    for (prev_str = NULL, str = sp_matr.matrix; str; prev_str = newstr, str = str->next) {
        newstr = new Sp_matr_str;
        newstr->index = str->index;
        newstr->first = NULL;
        newstr->next = NULL;
        if (prev_str == NULL) {
            this->matrix = newstr;
        }
        else {
            prev_str->next = newstr;
        } 
        for (prev_elem = NULL, elem = str->first; elem; prev_elem = newelem, elem = elem->next) {
            newelem = new Sp_matr_elem;
            newelem->index = elem->index;
            newelem->value = elem->value;
            newelem->next = NULL;
            if (prev_elem == NULL) {
                newstr->first = newelem;
            }
            else {
                prev_elem->next = newelem;
            }
        }
    }
}


#define SAFEACCESS\
    try {\
        if (index1 >= this->n || index2 >= this->m) {\
            throw "invalid coordinates";\
        }\
    }\
    catch(const char* er) {\
        std::cout << er << std::endl;\
        exit(3);\
    }


double SparseMatrix::get(size_t index1, size_t index2) const {
    SAFEACCESS;
    Sp_matr_str *str;
    for (str = this->matrix; str; str = str->next) {
        if (str->index == index1) {
            Sp_matr_elem *elem;
            for (elem = str->first; elem; elem = elem->next) {
                if (elem->index == index2) {
                    return elem->value;
                }
            }
            return 0;
        }
    }
    return 0;
}


void SparseMatrix::set(size_t index1, size_t index2, double value) {
    SAFEACCESS;
    Sp_matr_str *str;
    Sp_matr_elem *elem;
    for (str = this->matrix; str; str = str->next) {
        if (str->index == index1) {
            for (elem = str->first; elem; elem = elem->next) {
                if (elem->index == index2) {
                    elem->value = value;
                    return;
                }
            }
            elem = new Sp_matr_elem;
            elem->index = index2;
            elem->value = value;
            elem->next = str->first;
            str->first = elem;
            return;   
        }
    }
    str = new Sp_matr_str;
    str->index = index1;
    str->first = new Sp_matr_elem;
    str->first->index = index2;
    str->first->value = value;
    str->first->next = NULL;
    str->next = this->matrix;
    this->matrix = str;
}


void SparseMatrix::add(size_t index1, size_t index2, double value) {
    SAFEACCESS;
    Sp_matr_str *str;
    Sp_matr_elem *elem;
    for (str = this->matrix; str; str = str->next) {
        if (str->index == index1) {
            for (elem = str->first; elem; elem = elem->next) {
                if (elem->index == index2) {
                    elem->value += value;
                    return;
                }
            }
            elem = new Sp_matr_elem;
            elem->index = index2;
            elem->value = value;
            elem->next = str->first;
            str->first = elem;
            return;   
        }
    }
    str = new Sp_matr_str;
    str->index = index1;
    str->first = new Sp_matr_elem;
    str->first->index = index2;
    str->first->value = value;
    str->first->next = NULL;
    str->next = this->matrix;
    this->matrix = str;
}


size_t SparseMatrix::num_rows() const {
    return this->n;
}
    

size_t SparseMatrix::num_columns() const {
    return this->m;
}


bool SparseMatrix::operator ==(const SparseMatrix & sp_matr) const {
    if (this->n != sp_matr.n || this->m != sp_matr.m) {
        return false;
    }
    Sp_matr_str *str;
    Sp_matr_elem *elem;
    for (str = this->matrix; str; str = str->next) {
        for (elem = str->first; elem; elem = elem->next) {
            if (std::abs(elem->value - sp_matr.get(str->index, elem->index)) >= eps) {
                return false;
            }
        }
    }
    return true;
}


bool SparseMatrix::operator !=(const SparseMatrix & sp_matr) const {
    if (this->n != sp_matr.n || this->m != sp_matr.m) {
        return true;
    }
    Sp_matr_str *str;
    Sp_matr_elem *elem;
    for (str = this->matrix; str; str = str->next) {
        for (elem = str->first; elem; elem = elem->next) {
            if (std::abs(elem->value - sp_matr.get(str->index, elem->index)) >= eps) {
                return true;
            }
        }
    }
    return false;
}


SparseMatrix SparseMatrix::operator +(const SparseMatrix & sp_matr) const {
    try {
        if (this->n != sp_matr.n || this->m != sp_matr.m) {
            throw 1;
        }
    }
    catch(int i) {
        std::cout << "invalid dimensions" << std::endl;
        exit(1);
    }
    SparseMatrix matr = sp_matr;
    Sp_matr_str *str = this->matrix;
    Sp_matr_elem *elem;
    for (str = this->matrix; str; str = str->next) {
        for (elem = str->first; elem; elem = elem->next) {
            matr.add(str->index, elem->index, elem->value);
        }
    }
    return matr;
}


SparseMatrix SparseMatrix::operator *(const SparseMatrix & sp_matr) const {
    try {
        if (this->m != sp_matr.n) {
            throw 2;
        }
    }
    catch(int i) {
        std::cout << "invalid dimensions" << std::endl;
        exit(2);
    }
    SparseMatrix matr(this->n, sp_matr.m);
    Sp_matr_str *str1, *str2;
    Sp_matr_elem *elem1, *elem2;
    for (str1 = this->matrix; str1; str1 = str1->next) {
        for (elem1 = str1->first; elem1; elem1 = elem1->next) {
            for (str2 = sp_matr.matrix; str2; str2 = str2->next) {
                if (elem1->index == str2->index) {
                    for (elem2 = str2->first; elem2; elem2 = elem2->next) {
                        matr.add(str1->index, elem2->index, elem1->value * elem2->value);
                    }
                }
            }
        }
    }
    return matr;
}


SparseMatrix & SparseMatrix::operator =(const SparseMatrix & sp_matr) {
    if (this == &sp_matr) {
        return *this;
    }
    while (this->matrix) {
        Sp_matr_str *str = this->matrix;
        while (str->first) {
            Sp_matr_elem *elem = str->first;
            str->first = str->first->next;
            delete elem;
        }
        this->matrix = this->matrix->next ;
        delete str;
    }
    this->matrix = NULL;
    this->n = sp_matr.n;
    this->m = sp_matr.m;
    Sp_matr_str *str, *prev_str, *newstr;
    Sp_matr_elem *elem, *prev_elem, *newelem;      
    for (prev_str = NULL, str = sp_matr.matrix; str; prev_str = newstr, str = str->next) {
        newstr = new Sp_matr_str;
        newstr->index = str->index;
        newstr->first = NULL;
        newstr->next = NULL;
        if (prev_str == NULL) {
            this->matrix = newstr;
        }
        else {
            prev_str->next = newstr;
        } 
        for (prev_elem = NULL, elem = str->first; elem; prev_elem = newelem, elem = elem->next) {
            newelem = new Sp_matr_elem;
            newelem->index = elem->index;
            newelem->value = elem->value;
            newelem->next = NULL;
            if (prev_elem == NULL) {
                newstr->first = newelem;
            }
            else {
                prev_elem->next = newelem;
            }
        }
    }
    return *this;
}


void SparseMatrix::set_eps(double e) {
    eps = e;
}


SparseMatrix::Intermediate_object SparseMatrix::operator [](int index) {
    return Intermediate_object(this, index);
}


const SparseMatrix::Intermediate_object SparseMatrix::operator [](int index) const {
    return Intermediate_object(this, index);
}


SparseMatrix::Intermediate_object SparseMatrix::operator *() {
    return Intermediate_object(this, 0);
}


const SparseMatrix::Intermediate_object SparseMatrix::operator *() const {
    return Intermediate_object(this, 0);
}


SparseMatrix::Offset_intermediate_object SparseMatrix::operator +(int c) {
    return Offset_intermediate_object(this, c);
}


const SparseMatrix::Offset_intermediate_object SparseMatrix::operator +(int c) const {
    return Offset_intermediate_object(this, c);
}


SparseMatrix::Indexed_element::operator double() {
    return this->matrix_ptr->get(this->index1, this->index2);
}
        
double SparseMatrix::Indexed_element::operator =(double c) {
    this->matrix_ptr->set(this->index1, this->index2, c);
    return c;
}
double SparseMatrix::Indexed_element::operator +=(double c) {
    this->matrix_ptr->add(this->index1, this->index2, c);
    return c;
}
double SparseMatrix::Indexed_element::operator -=(double c) {
    this->matrix_ptr->add(this->index1, this->index2, -c);
    return c;
}
double SparseMatrix::Indexed_element::operator *=(double c) {
    double d = this->matrix_ptr->get(this->index1, this->index2);
    this->matrix_ptr->set(this->index1, this->index2, c * d);
    return c;
}
double SparseMatrix::Indexed_element::operator /=(double c) {
    double d = this->matrix_ptr->get(this->index1, this->index2);
    this->matrix_ptr->set(this->index1, this->index2, d / c);
    return c;
}


SparseMatrix::Indexed_element SparseMatrix::Offset_indexed_element::operator *() {
    return Indexed_element(this->matrix_ptr, this->index1, this->index2);
}


double SparseMatrix::Offset_indexed_element::operator *() const {
    return const_matrix_ptr->get(this->index1, this->index2);
}


SparseMatrix::Indexed_element SparseMatrix::Intermediate_object::operator [](int i) {
    return Indexed_element(this->matrix_ptr, this->index, i);
}


double SparseMatrix::Intermediate_object::operator [](int i) const {
    return const_matrix_ptr->get(this->index, i);
}


SparseMatrix::Indexed_element SparseMatrix::Intermediate_object::operator *() {
    return Indexed_element(this->matrix_ptr, this->index, 0);
}


double SparseMatrix::Intermediate_object::operator *() const {
    return const_matrix_ptr->get(this->index, 0);
}


SparseMatrix::Offset_indexed_element SparseMatrix::Intermediate_object::operator +(int c) {
    return Offset_indexed_element(this->matrix_ptr, this->index, c);
}


const SparseMatrix::Offset_indexed_element SparseMatrix::Intermediate_object::operator +(int c) const {
    return Offset_indexed_element(this->const_matrix_ptr, this->index, c);
}


SparseMatrix::Intermediate_object SparseMatrix::Offset_intermediate_object::operator *() {
    return Intermediate_object(this->matrix_ptr, this->index);
}


const SparseMatrix::Intermediate_object SparseMatrix::Offset_intermediate_object::operator *() const {
    return Intermediate_object(this->const_matrix_ptr, this->index);
}


SparseMatrix::~SparseMatrix() {
    while (this->matrix) {
        Sp_matr_str *str = this->matrix;
        while (str->first) {
            Sp_matr_elem *elem = str->first;
            str->first = str->first->next;
            delete elem;
        }
        this->matrix = this->matrix->next;
        delete str;
    }
}
