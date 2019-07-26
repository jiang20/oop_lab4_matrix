//
// Created by cxz on 2019/6/2.
//

#ifndef OOP_LAB4_MATRIX_MATRIX_H
#define OOP_LAB4_MATRIX_MATRIX_H

#include "Bignum.h"

class Matrix {
    const static int Width = 3;
    Bignum bignum[Width][Width];
    int maxLength;
public:
    Matrix(Bignum bignum[Width][Width]);

    Matrix();
    void out();
    Matrix operator+(const Matrix& matrix);
    Matrix operator-(const Matrix& matrix);
    Matrix operator*(const Matrix& matrix);
    Matrix operator*(const long& number);
    Matrix operator~();
    Matrix operator+=(const Matrix& matrix);
    Matrix operator-=(const Matrix& matrix);
    Matrix operator=(const Matrix& matrix);
    int getMaxLength() const ;
    void setMaxLength(int maxLength) ;
    void setBignum(int index , const Bignum& bignum1);
    void clear();
};
Matrix operator*(const long& number , const Matrix& matrix);

#endif //OOP_LAB4_MATRIX_MATRIX_H
