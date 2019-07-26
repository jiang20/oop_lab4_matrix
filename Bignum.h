//
// Created by cxz on 2019/6/2.
//

#ifndef OOP_LAB4_MATRIX_BIGNUM_H
#define OOP_LAB4_MATRIX_BIGNUM_H


#include <string>

class Bignum {

    int size;
    bool sign;
public:
    int bigNum[128];
    void out();
    Bignum(std::string& explicitString);
    Bignum();
    Bignum(int num);
    ~Bignum();
    int getSize() const;
    void setSize(int size);
    void setSign(bool sign);
    void setNum(int index , int number);
    Bignum sToBignum(std::string number);
    const Bignum operator+(const Bignum& bignum1);
    const Bignum operator-(const Bignum& bignum1);
    const Bignum operator*(const Bignum& bignum1);
    const Bignum operator+=(const Bignum& bignum1);
    const Bignum operator-=(const Bignum& bignum1);
    const bool operator>(const Bignum& bignum1);
    const bool operator==(const Bignum& bignum1);
    const Bignum operator=(const Bignum& bignum1);
};


#endif //OOP_LAB4_MATRIX_BIGNUM_H
