//
// Created by cxz on 2019/6/2.
//

#include "Bignum.h"
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

static bool getSign(string &explicitString);

static void setBigNum(int bigNum[], int size, string &explicitString);

/*size是包含数字的一块，正负号没有进行考虑*/
Bignum::Bignum(std::string &explicitString) {
    sign = getSign(explicitString);
    size = explicitString.size();
    setBigNum(bigNum, size, explicitString);
}

Bignum::Bignum(){
    size = 0;
    sign = false;
    string a("0");
    setBigNum(bigNum , 0 , a);
};

Bignum::Bignum(int num) {
    int tmp = (num < 0) ? -num:num;
    int tmp2 = tmp;
    sign = (num >= 0);
    int sizeNow = 0;
    while (tmp > 0){
        tmp /= 10;
        size++;
    }
    this->size = sizeNow;
    for (int i = 0; i < size; ++i) {
        bigNum[size - 1 - i] = tmp2 % 10;
        tmp2 /= 10;
    }
}

void Bignum::out() {
    if (sign)
        cout << ' ';
    else
        cout << '-';
    if(size == 0){
        cout << 0;
    } else {
        for (int i = 0; i < size; i++) {
            cout << bigNum[i];
        }
    }
}

int Bignum::getSize() const {
    return size;
}
void Bignum::setSize(int size) {
    this->size = size;
}
//42435+158652
const Bignum Bignum::operator+(const Bignum &bignum1) {
    Bignum bignumResult;
    int rest = 0;
    if (sign && bignum1.sign) {
        int newSize = (size > bignum1.size) ? size : bignum1.size;
        for (int i = newSize - 1, j = 0; i >= 0; i--, j++) {
            int sum = ((size - j - 1) >= 0 ? bigNum[size - j - 1] : 0) + ((bignum1.size - j - 1) >= 0 ? bignum1.bigNum[bignum1.size - j - 1]: 0);
            bignumResult.bigNum[i] = (sum + rest) % 10;
            rest = (sum +rest) / 10;
        }
        if (rest > 0) {
            for (int i = newSize; i >= 0; i--) {
                bignumResult.bigNum[i + 1] = bignumResult.bigNum[i];
            }
            bignumResult.bigNum[0] = rest;
            newSize++;
        }
        bignumResult.setSign(true);
        bignumResult.setSize(newSize);
    }
    /*正数加负数，转化为正数减正数*/
    else if(sign && !bignum1.sign){
        Bignum bignum2 = bignum1;
        bignum2.setSign(true);
        bignumResult = *(Bignum *)this - bignum2;
    }
    /*负数加正数转化为-（正数减正数）*/
    else if(!sign && bignum1.sign){
        this->sign = true;
        bignumResult = *(Bignum *)this - bignum1;
        this->sign = false;
        bignumResult.setSign(!bignumResult.sign);
    }
    /*负数加负数转化为-（正数加正数）*/
    else{
        Bignum bignum2 = bignum1;
        bignum2.sign = true;
        this->sign = true;
        bignumResult = *(Bignum *)this + bignum2;
        bignumResult.sign = false;
        this->sign = false;
    }
    return bignumResult;
}

const Bignum Bignum::operator-(const Bignum &bignum1) {
    Bignum bignumResult;
    if(this->sign && bignum1.sign){
        if(*this > bignum1){
            bignumResult.sign = true;
            int minSize = (bignum1.getSize() < this->getSize()) ? bignum1.getSize() : this->getSize();
            int bignum1Size = bignum1.getSize();
            int differSign = 0;
            for(int i = this->getSize() - 1 , j = 0 ; i >= 0 ; i-- , j++){
                int differ;
                if(bignum1Size - j  - 1>= 0 )
                    differ = this->bigNum[i] - bignum1.bigNum[bignum1Size - j - 1];
                else
                    differ = this->bigNum[i];
                bignumResult.bigNum[i] = (differ + 10 + differSign) % 10;
                if(bignum1Size - j - 1 >= 0)
                    differSign = (this->bigNum[i] < bignum1.bigNum[bignum1Size - j - 1]) ? -1:0;
            }
            bignumResult.setSize(this->getSize());
            if(bignumResult.bigNum[0] == 0){
                for(int i = 0 ; i < this->getSize() - 2 ; i++){
                    bignumResult.bigNum[i] = bignumResult.bigNum[i+1];
                }
                bignumResult.setSize(this->getSize() - 1);
            }
        }
        else if(*this == bignum1)
            return bignumResult;
        else{
            Bignum bignum2 = bignum1;
            bignumResult = bignum2 - *this;
            bignumResult.sign = false;
        }
    }
    else if(this->sign && !bignum1.sign) {
        Bignum bignum2 = bignum1;
        bignum2.sign = true;
        bignumResult = *this + bignum2;
        bignumResult.sign = true;
    }
    else if(!this->sign && bignum1.sign){
        this->sign = true;
        bignumResult = *this + bignum1;
        bignumResult.sign = false;
        this->sign = false;
    }
    else{
        Bignum bignum2 = bignum1;
        bignum2.sign = true;
        bignumResult = *this + bignum2;
    }
    return bignumResult;
}

const Bignum Bignum::operator*(const Bignum &bignum1) {
    Bignum bignumResult;
    if(this->sign == bignum1.sign)
        bignumResult.sign = true;
    int resultNow[this->getSize() + bignum1.getSize()];
    /*理论上最大的长度，但是可能没有*/
    int resultSize = this->getSize() + bignum1.getSize();
    int sizeNow = this->getSize();
    int sizeBignum1 = bignum1.getSize();
    vector<int> arrays[resultSize];
    int rest = 0;
    for(int j = sizeBignum1 - 1 , i = 0 ; j >= 0; j--){
        rest = 0;
        for(i = sizeNow - 1 ; i >= 0; i--){
            int product = this->bigNum[i] * bignum1.bigNum[j];
            arrays[resultSize - sizeBignum1 + j - sizeNow +i + 1].push_back((product + rest) % 10);
            rest = ( product + rest) / 10;
        }
        arrays[resultSize - sizeBignum1 + j - sizeNow +i + 1].push_back(rest);
    }
    int restTwice = 0;
    for(int i = resultSize - 1 ; i >= 0; i--){
        int amount = 0;
        for(int number : arrays[i]){
            amount += number;
        }
        int add = amount + restTwice;
        resultNow[i] =  add % 10;
        restTwice = add / 10;
    }
    /*最终真正的结果的长度*/
    int sizeResult = (resultNow[0] == 0) ? resultSize - 1 :resultSize;
    bignumResult.setSize(sizeResult);
    for(int i = 0 ; i <= sizeResult - 1 ; i++){
        bignumResult.bigNum[sizeResult - 1 - i] = resultNow[resultSize - i - 1];
    }
    return bignumResult;
}

Bignum const Bignum::operator+=(const Bignum &bignum1) {
    *this = *this + bignum1;
    return *this;
}

const Bignum Bignum::operator-=(const Bignum &bignum1) {
    *this =  *this - bignum1;
    return *this;
}
bool const Bignum::operator>(const Bignum& bignum1){
    /*前正后负*/
    if(this->sign && !bignum1.sign)
        return true;
    /*前负后正*/
    if(!this->sign && bignum1.sign)
        return false;
    /*相同长度*/
    if((this->getSize() == bignum1.getSize())){
        int sizeHere = this->getSize();
        for(int i = 0 ; i < sizeHere ; i++){
            if(this->bigNum[i] != bignum1.bigNum[i])
                return this->bigNum[i] > bignum1.bigNum[i];
        }
    }
    /*长度不同、正负相同的情况*/
    return (this->getSize() > bignum1.getSize() == this->sign);
}

const bool Bignum::operator==(const Bignum& bignum1){
    if(this->sign == bignum1.sign && this->getSize() == bignum1.getSize()){
        int sizeHere = this->getSize();
        for(int i = 0 ; i < sizeHere ; i ++){
            if(this->bigNum[i] != bignum1.bigNum[i])
                return false;
            if(i == sizeHere - 1)
                return true;
        }
    }
    return false;
}

const Bignum Bignum::operator=(const Bignum &bignum1) {
    this->sign = bignum1.sign;
    this->setSize(bignum1.getSize());
    int sizeNow = bignum1.getSize();
    for(int i  = 0 ; i < sizeNow ; i++){
        this->bigNum[i] = bignum1.bigNum[i];
    }
    return *this;
}
Bignum::~Bignum() = default;


static bool getSign(string &explicitString) {
    bool sign = true;
    if (!explicitString.empty()) {
        int first_num = explicitString.at(string::size_type(0));
        if (first_num == '-') {
            sign = false;
            explicitString = explicitString.substr(string::size_type(1));
        }
    }
    return sign;
}

void Bignum::setSign(bool sign) {
    this->sign = sign;
}

static void setBigNum(int bigNum[], int size, string &explicitString) {
    for (int i = 0; i < size; i++) {
        bigNum[i] = explicitString.at(i) - '0';
    }
}

void Bignum::setNum(int index, int number) {
    bigNum[index] = number;
}

Bignum Bignum::sToBignum(string number) {
    Bignum resultBignum;
    resultBignum.sign = (number.at(0) != '-');
    resultBignum.setSize(number.size());
    int i = (resultBignum.sign)? 0 : 1;
    for( ; i < number.size() ; i++){
        resultBignum.setNum(i ,( number.at(i) - '0'));
    }
    return resultBignum;
}




