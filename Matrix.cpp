//
// Created by cxz on 2019/6/2.
//

#include "Matrix.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cstring>

using namespace std;

Matrix::Matrix(Bignum bignum[Width][Width]){
    maxLength = 0;
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            this->bignum[i][j] = bignum[i][j];
            if(bignum[i][j].getSize() > maxLength)
                maxLength = bignum[i][j].getSize();
        }
    }
}


Matrix::Matrix(){
    maxLength = 0;
    Bignum bignum1;
    for(auto & i : bignum){
        for (auto & j : i) {
            j = bignum1;
        }
    }
}

Matrix Matrix::operator+(const Matrix &matrix) {
    Matrix resultMatrix;
    int maxLengthHere = 0;
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0; j < 3 ; j++){
            resultMatrix.bignum[i][j] = this->bignum[i][j] + matrix.bignum[i][j];
            int maxNow = resultMatrix.bignum[i][j].getSize();
            maxLengthHere = (maxNow > maxLengthHere) ? maxNow : maxLengthHere;
        }
    }
    resultMatrix.setMaxLength(maxLengthHere);
    return resultMatrix;
}

Matrix Matrix::operator-(const Matrix &matrix) {
    Matrix resultMatrix;
    int maxLengthHere = 0;
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0; j < 3 ; j++){
            resultMatrix.bignum[i][j] = this->bignum[i][j] - matrix.bignum[i][j];
            int maxNow = resultMatrix.bignum[i][j].getSize();
            maxLengthHere = (maxNow > maxLengthHere) ? maxNow : maxLengthHere;
        }
    }
    resultMatrix.setMaxLength(maxLengthHere);
    return resultMatrix;
}

Matrix Matrix::operator*(const Matrix &matrix) {
    Matrix resultMatrix;
    //resultMatrix.clear();
    int maxLengthHere = 0;
//    Bignum bignum1;
//    Bignum bignum2;
    for(int i = 0 ; i < 3 ;i++){
        for(int j = 0 ; j < 3 ; j++){
            resultMatrix.bignum[i][j] += this->bignum[i][0] * matrix.bignum[0][j];
            resultMatrix.bignum[i][j] += this->bignum[i][1] * matrix.bignum[1][j];
            resultMatrix.bignum[i][j] += this->bignum[i][2] * matrix.bignum[2][j];
            maxLengthHere = (resultMatrix.bignum[i][j].getSize() > maxLengthHere) ? resultMatrix.bignum[i][j].getSize() : maxLengthHere;
        }
    }
    resultMatrix.setMaxLength(maxLengthHere);
    return resultMatrix;
}

Matrix Matrix::operator*(const long &number) {
    Matrix resultMatrix;
    Bignum bignum1;
    if(number >= 0)
        bignum1.setSign(true);
    int times = 0;
    int result = 1 , numberPlus = (number > 0) ? (number) : (-1 * number);
    while(result != 0 ){
        times++;
        result= (int)(numberPlus / pow(10 , times));
    }
    bignum1.setSize(times);
    //或许考虑把大数乘矩阵变成矩阵乘矩阵！
    for(int i = 0 ; i < times ; i++){
        int numberNew = (int)(numberPlus / pow(10 , i))%10;
        bignum1.setNum(times - 1 - i , numberNew);
    }
    int maxLengthHere = 0;
    for(int i = 0 ; i < 3 ; i++) {
        for (int j = 0 ; j < 3 ; j++) {
            resultMatrix.bignum[i][j] = this->bignum[i][j] * bignum1;
            maxLengthHere = (maxLengthHere > resultMatrix.bignum[i][j].getSize()) ? maxLengthHere : resultMatrix.bignum[i][j].getSize();
        }
    }
    resultMatrix.setMaxLength(maxLengthHere);
    return resultMatrix;
}
Matrix operator*(const long& number , const Matrix& matrix){
    Matrix matrix1 = matrix;
    Matrix resultMatrix;
    resultMatrix = matrix1 * number;
    return resultMatrix;
}

Matrix Matrix::operator~() {
    Bignum bignum1;
    for(int i = 0 ; i < 3 ; i++ ){
        for(int j = i + 1; j < 3 ; j ++){
            bignum1 = this->bignum[i][j];
            this->bignum[i][j] = this->bignum[j][i];
            this->bignum[j][i] = bignum1;
        }
    }
    return *this;
}

Matrix Matrix::operator+=(const Matrix &matrix) {
    int maxLengthHere = 0;
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            this->bignum[i][j] += matrix.bignum[i][j];
            maxLengthHere = (this->bignum[i][j].getSize() > maxLengthHere) ? this->bignum[i][j].getSize() : maxLengthHere;
        }
    }
    this->setMaxLength(maxLengthHere);
    return *this;
}

Matrix Matrix::operator-=(const Matrix &matrix) {
    int maxLengthHere = 0;
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            this->bignum[i][j] -= matrix.bignum[i][j];
            maxLengthHere = (this->bignum[i][j].getSize() > maxLengthHere) ? this->bignum[i][j].getSize() : maxLengthHere;
        }
    }
    this->setMaxLength(maxLengthHere);
    return *this;
}

Matrix Matrix::operator=(const Matrix &matrix) {
    int maxLengthHere = 0;
    for(int i = 0 ; i < 3 ; i++){
        for (int j = 0; j < 3; ++j) {
            this->bignum[i][j] = matrix.bignum[i][j];
            maxLengthHere = (this->bignum[i][j].getSize() > maxLengthHere) ? this->bignum[i][j].getSize() : maxLengthHere;
        }
    }
    this->setMaxLength(maxLengthHere);
    return *this;
}
void Matrix::out(){
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++) {
             string empty(maxLength - bignum[i][j].getSize() , ' ');
             cout<<empty;
             bignum[i][j].out();
             if(j != 3) cout<<" ";
        }
        cout<<endl;
    }
}

void Matrix::clear() {
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            memset( bignum[i][j].bigNum, '\0' , 128);
            bignum[i][j].setSize(0);
            bignum[i][j].setSign(false);
        }
    }
    this->setMaxLength(0);
}

int Matrix::getMaxLength() const {
    return maxLength;
}

void Matrix::setMaxLength(int maxLengthHere){
    this->maxLength = maxLengthHere;
}

void Matrix::setBignum(int index, const Bignum& bignum1) {
    this->bignum[index / 3][index % 3] = bignum1;
    int maxMatrixLength = 0;
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0; j < 3 ; j++){
            maxMatrixLength = ( this->bignum[i][j].getSize() > maxMatrixLength ) ? this->bignum[i][j].getSize() : maxMatrixLength;
        }
    }
    maxLength = maxMatrixLength;
}