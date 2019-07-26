//
// Created by cxz on 2019/6/5.
//

#ifndef OOP_LAB4_MATRIX_SYMBOL_H
#define OOP_LAB4_MATRIX_SYMBOL_H

#include <iostream>
#include <stack>

class Symbol {
    std::string content;
    bool isMatrix;
    bool isNumber;
    bool isOperation;
    int order;
public:
    Symbol();
    std::string getContent();
    bool ifMatrix();
    bool ifNumber();
    bool ifOperation();
    int getOrder();

    void setContent(std::string content);
    void setIsMatrix(bool isMatrix);
    void setIsNumber(bool isNumber);
    void setIsOperation(bool isOperation);
    void setOrder(int order);
    void setAllProperties(std::string content , bool isMatrix , bool isNumber , bool isOperation , int order);
};


#endif //OOP_LAB4_MATRIX_SYMBOL_H
