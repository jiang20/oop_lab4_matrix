//
// Created by cxz on 2019/6/5.
//

#include "Symbol.h"
#include <iostream>

using namespace std;

Symbol::Symbol() { order = 0; isOperation = true; isNumber = false; isMatrix = false;}

std::string Symbol::getContent() {
    return content;
}

bool Symbol::ifMatrix() {
    return isMatrix;
}

bool Symbol::ifNumber() {
    return isNumber;
}

bool Symbol::ifOperation() {
    return isOperation;
}

int Symbol::getOrder() {
    return order;
}

void Symbol::setContent(std::string content) {
    this->content = content;
}

void Symbol::setIsMatrix(bool isMatrix) {
    this->isMatrix  = isMatrix;
}

void Symbol::setIsNumber(bool isNumber) {
    this->isNumber = isNumber;
}

void Symbol::setIsOperation(bool isOperation) {
    this->isOperation = isOperation;
}

void Symbol::setOrder(int order) {
    this->order = order;
}

void Symbol::setAllProperties(std::string content, bool isMatrix, bool isNumber, bool isOperation, int order) {
    this->content = content;
    this->isMatrix = isMatrix;
    this->isNumber = isNumber;
    this->isOperation = isOperation;
    this->order = order;
}