#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <stdlib.h>
#include <stack>
#include <assert.h>
#include <cstring>
#include "Bignum.h"
#include "Matrix.h"
#include "Symbol.h"
using namespace std;



map<string , Matrix> solve_in(const string& fileIn);
vector<Symbol> stringToSymbols(string line);
bool isOperator(char sign );
bool isMinus(char sign , char signNext);
bool isNumber(char sign);
vector<Symbol>symbolsToSuffix(vector<Symbol> vectorSymbols , map<string , Matrix>& stringMatrixMap);
Matrix countFromSuffix(vector<Symbol> suffixExpression , map<string , Matrix> stringMatrixMap);
Matrix symbolToMatrix(Symbol symbol , map<string , Matrix> stringMatrixMap);
Matrix stringToMatrix(string line , map<string ,Matrix> stringMatrixMap);
vector<Matrix>solveFileCount(const string& fileIn , const string& expression);
long symbolToNumber(Symbol symbol);
Matrix countMatrixWithMatrix(Matrix matrix1 , Matrix matrix2 , Symbol symbol);
string charToString(char sign);
string numberToString(int number);
int stringToNumber(string string1);
vector<string> split(string s, string separator);
vector<string> split(string s, vector<string> seperatos);
void matrixAddLine(Matrix& matrix , vector<string> explicitMatrixLine , int stamp);
string toString(vector<string> strings);
const int LENGTH = 50;
enum {low = 1 , medium = 2 , high = 3 };


int main() {
    string fileIn = "matrix.in";
    string expression = "expression.in";
    vector<Matrix> resultMatrix = solveFileCount(fileIn , expression);
    for(auto & i : resultMatrix){
        i.out();
    }
}

/*从包含矩阵的文件和包含表达式的文件得到所有结果*/
vector<Matrix>solveFileCount(const string& fileIn , const string& expression){
    map<string , Matrix> stringMatrixMap = solve_in(fileIn);
    ifstream in;
    in.open(expression);
    char line[LENGTH];
    Matrix matrix;
    vector<Matrix> resultMatrix;
    while(in.getline(line,LENGTH)){
        matrix = stringToMatrix(line , stringMatrixMap);
        resultMatrix.push_back(matrix);
        memset(line,0,LENGTH);
    }
    return resultMatrix;
}

/*将string利用map<string , Matrix>转化为相应矩阵*/
Matrix stringToMatrix(string line , map<string ,Matrix> stringMatrixMap){
    vector<Symbol> symbols = stringToSymbols(line);
    vector<Symbol> symbolsSuffix = symbolsToSuffix(symbols , stringMatrixMap);
    Matrix matrix = countFromSuffix(symbolsSuffix , stringMatrixMap);
    return matrix;
}

/*将原本的字符串格式转化为Symbol格式*/
vector<Symbol> stringToSymbols(string line){
    vector<Symbol> symbols;
    Symbol symbol;
    for(int i  = 0 ; i < line.length() ; i++){
        char charNow = line.at(i);
        char charNext = 0;
        if(i + 1 < line.length())
            charNext = line.at(i+1);
//        if(i == 0 && isOperator(charNow) && isNumber(charNext)){
//            symbol.setContent("-" + )
//        }
        if( i == 0 && isMinus(charNow , charNext)){
            int j;
            for(j = 1 ; j < line.length() - i ; j++){
                if(!isNumber(line.at(j)))
                    break;
            }
            i = i + j - 1;
            symbol.setAllProperties(line.substr(0,j) , false , true , false , 0);
        }else if(charNow == '~'){
            i++;
            symbol.setAllProperties("~" + charToString(charNext) , true , false , false , 0);
        }else if(isOperator(charNow) && (i + 1 ) < line.length() && !isNumber(charNext)) {
            symbol.setAllProperties(charToString(charNow) , false , false , true , 0);
            if (charNow == '+' || charNow == '-')
                symbol.setOrder(low);
            else if (charNow == '*')
                symbol.setOrder(medium);
            else
                symbol.setOrder(high);
        }else if(isNumber(charNow)){
            symbol.setAllProperties(charToString(charNow) , false , true , false , 0);
        }else{
            symbol.setAllProperties(charToString(line.at(i)), true , false , false , 0);
        }
        symbols.push_back(symbol);
    }
    return symbols;
}

bool isOperator(char sign ){
    return (sign == '+' || (sign == '-')|| sign == '*');
}

bool isMinus(char signNow , char signNext){
    return ((signNow == '-') && isNumber(signNext));
}

bool isNumber(char sign){
    return (sign >= '0' && sign <= '9');
}

/*将原本表达式（symbol形式）转化为后缀表达式形式*/
vector<Symbol>symbolsToSuffix(vector<Symbol> vectorSymbols  , map<string , Matrix>& stringMatrixMap){
    Symbol preSymbol , nowSymbol;
    vector<Symbol> resultVector;
    stack<Symbol> aStack;
    for(int i = 0 ; i < vectorSymbols.size() ; i++){
        if(!aStack.empty()) preSymbol = aStack.top();
        nowSymbol = vectorSymbols.at(i);

        /*当第一位是运算符时，再前面加一个空的矩阵*/
        if(nowSymbol.ifOperation() && i == 0){
            Matrix zeroMatrix;
            string zeroMatrixName = "0";
            stringMatrixMap.insert(pair<string , Matrix> (zeroMatrixName , zeroMatrix));
            Symbol symbol;
            symbol.setIsMatrix(true);
            symbol.setIsNumber(false);
            symbol.setIsOperation(false);
            symbol.setContent(zeroMatrixName);
            symbol.setOrder(0);
            resultVector.push_back(symbol);
        }
        if(nowSymbol.ifOperation() ){
            if(aStack.empty() || nowSymbol.getOrder() > preSymbol.getOrder())
                aStack.push(nowSymbol);
            else{
                while (!aStack.empty()){
                    if(aStack.top().getOrder() >= nowSymbol.getOrder()){
                        resultVector.push_back(aStack.top());
                        aStack.pop();
                    }
                    else
                        break;
                }
                aStack.push(nowSymbol);
            }
        }
        else {
            resultVector.push_back(nowSymbol);
        }
    }
    /*保证stack中所有的运算符都被加入到后缀表达式中*/
    while (!aStack.empty()){
        resultVector.push_back(aStack.top());
        aStack.pop();
    }
    return resultVector;
}

/*从后缀表达式，即逆波兰式得到运算结果*/
Matrix countFromSuffix(vector<Symbol> suffixExpression , map<string , Matrix> stringMatrixMap){
    Matrix resultMatrix;
    stack<Symbol> countingStack;
    Symbol nowSymbol;
    Symbol leftSign;
    Symbol rightSign;

    Matrix tmp_other;
    long numberTmp;
    int newMatrixNum = 0;
    string newMatrixName;
    for(int i = 0 ; i < suffixExpression.size() ; i++){
        Matrix tmp;
        nowSymbol = suffixExpression.at(i);
        if(i == 0 && suffixExpression.size() >1 && suffixExpression.at(1).ifOperation() && suffixExpression.at(1).getContent() == "-" && !nowSymbol.ifOperation()){
            newMatrixNum++;
            tmp = symbolToMatrix(suffixExpression.at(0) , stringMatrixMap);
            tmp = (-1) * tmp;
            i++;
            newMatrixName = numberToString(newMatrixNum);
            stringMatrixMap.insert(pair<string , Matrix>(newMatrixName , tmp));
            Symbol symbol;
            /*更新map，得到symbol*/
            symbol.setContent(newMatrixName);
            symbol.setIsMatrix(true);
            symbol.setIsNumber(false);
            symbol.setIsOperation(false);
            countingStack.push(symbol);
        }
        else if(!nowSymbol.ifOperation()){
            countingStack.push(nowSymbol);
        }
        else{
            newMatrixNum++;
            if(nowSymbol.getContent() == "~"){
                rightSign = countingStack.top();
                tmp = symbolToMatrix(rightSign , stringMatrixMap);
                tmp = ~tmp;
            }
            else {
                rightSign = countingStack.top();
                countingStack.pop();
                leftSign = countingStack.top();
                countingStack.pop();
                if (rightSign.ifNumber() || leftSign.ifNumber()) {
                    if (rightSign.ifNumber()) {
                        numberTmp = symbolToNumber(rightSign);
                        tmp = symbolToMatrix(leftSign, stringMatrixMap);
                    } else {
                        numberTmp = symbolToNumber(leftSign);
                        tmp = symbolToMatrix(rightSign, stringMatrixMap);
                    }
                    tmp = tmp * numberTmp;
                } else {
                    tmp.clear();
                    tmp_other.clear();
                    tmp = symbolToMatrix(leftSign, stringMatrixMap);
                    tmp_other = symbolToMatrix(rightSign, stringMatrixMap);
                    tmp = countMatrixWithMatrix(tmp, tmp_other, nowSymbol);
                }
            }
            newMatrixName = numberToString(newMatrixNum);
            stringMatrixMap.insert(pair<string , Matrix>(newMatrixName , tmp));
            Symbol symbol;
            /*更新map，得到symbol*/
            symbol.setContent(newMatrixName);
            symbol.setIsMatrix(true);
            symbol.setIsNumber(false);
            symbol.setIsOperation(false);
            countingStack.push(symbol);
        }
    }
    assert( !countingStack.empty());
    resultMatrix = stringMatrixMap[countingStack.top().getContent()];
    return resultMatrix;
}

/*将matrix利用得到的运算符进行计算，返回运算结果*/
Matrix countMatrixWithMatrix(Matrix matrix1 , Matrix matrix2 , Symbol symbol){
    string operation = symbol.getContent();
    if(operation == "+")
        return matrix1+matrix2;
    else if(operation == "-") {
        matrix1 - matrix2;
        int i;
        return matrix1 - matrix2;
    }
    return matrix1*matrix2;
}

/*将isMatrix的Symbol变量转化为矩阵*/
Matrix symbolToMatrix(Symbol symbol , map<string , Matrix> stringMatrixMap){
    Matrix result = stringMatrixMap[symbol.getContent()];
    return result;
}

/*将isNumber的Symbol变量转化为数字*/
long symbolToNumber(Symbol symbol){
    long result = 0;
    string numberString = symbol.getContent();
    for(int i = 0; i < numberString.size() ; i++){
        result = result * 10 + numberString.at(i) - '0';
    }
    return result;
}

/*将char类型的数据转化为string*/
string charToString(char sign){
    char c[1] = {sign};
    string result = c;
    return result;
}

/*将int类型的数据转化为string*/
string numberToString(int number){
    char x = (char) number;
    char c[1] = {x};
    string s = c;
    return s;
}

int stringToNumber(string string1){
    int result = 0;
    for(int i = 0 ; i < string1.length() ; i++){
        result = result * 10 + (string1.at(i) - '0');
    }
    return result;
}

////void matrixOperation(const string& fileIn , const string& operation , const string& fileOut) {
////    map<string , Matrix> matrixAll = solve_in(fileIn);
////    ifstream operations;
////    operations.open(operation);
////    char line[LENGTH];
////    vector<string> matrix_names;
////    for(pair aPair:matrixAll){
////        matrix_names.push_back(aPair.first);
////    }
////    vector<string> operators;
////    while (operations.getline(line , LENGTH)) {
////        string lineString = line;
////        operators = split(line, matrix_names);
////        //对于是否有~的要单独讨论，因为有可能出现+~这种，通过if判断是否包含来解决~
////        operators = solve_trans(operators);//考虑+和~前后出现的情况，对此应该进行继续分割
////        int number = operators.size();
////        stack<int> aStack;
////        map<int ,string> opeMap;
////        int index = 0;
////        int oldpre = 0;
////        for (int i = 0; i < number; i++) {
////            index = oldpre + lineString.substr(index).find(operators.at(i));
////            oldpre += index;
////            pair<int ,string> opePair(index , operators.at(i) );
////            opeMap.insert(opePair);
////        }
////        for(pair aPairOpe:opeMap){
////            if(aPairOpe.second == "+=" || aPairOpe.second == "-=")
////                aStack.push(aPairOpe.first);
////        }
////        for(pair aPairOpe:opeMap){
////            if(aPairOpe.second == "+" || aPairOpe.second == "-")
////                aStack.push(aPairOpe.first);
////        }
////        for(pair aPairOpe:opeMap){
////            if(aPairOpe.second == "*" )
////                aStack.push(aPairOpe.first);
////        }
////        for(pair aPairOpe:opeMap){
////            if(aPairOpe.second == "~")
////                aStack.push(aPairOpe.first);
////        }
////        solve_operation(line , aStack);
////    }
////}
//
////似乎只能用那个方法来实现了，对于数字和矩阵的处理只能
//
//
//vector<Matrix> solve_operation(string line, stack<int> aStack , map<string , Matrix> myMap){
//    string newLine = line;
//    int index;
//    int oldLength;
//    string matrixLeftName;
//    string matrixRightName;
//    Matrix matrixLeft;
//    Matrix matrixRight;
//    while (!aStack.empty()) {
//        index = aStack.top();
//        aStack.pop();
//        if (line.at(index) == '~') {
//            matrixRightName = line.at(index + 1);
//            matrixRight = myMap[matrixRightName];
//            matrixRight = ~matrixRight;
//            if(index == 0)
//                newLine = newLine.substr(index + 1);
//            newLine = newLine.substr(0, index - 1)+newLine.substr(index + 1);
//        } else{
//            matrixLeftName = line.at(index - 1);
//            matrixRightName = line.at(index + 1);
//            matrixLeft = myMap[matrixLeftName];
//            matrixRight = myMap[matrixRightName];
//            switch (line.at(index)){
//                case '+':matrixLeft = matrixLeft + matrixRight;
//                    break;
//                //case '*':
//            }
//        }
//    }
//}


/*对matrix.in文件进行处理，得到所有的矩阵，并存在一个map里*/
map<string , Matrix> solve_in(const string& fileIn){
    ifstream in;
    in.open(fileIn);
    int stamp = 0;
    map<string, Matrix> matrix_string;
    int matrix_number = 0;
    string nameTmp;
    char line[LENGTH];
    string lineString;
    vector<string> explicitMatrixLine;
    vector<string> matrixName;
    Matrix matrixTmp;
    while (in.getline(line, LENGTH)) {
        if (stamp == 0)
            matrix_number = line[0] - '0';
        else if (stamp % 4 == 1) {
            nameTmp = line;
        }
        else if (stamp % 4 == 2 || stamp % 4 == 3 ||stamp % 4 == 0) {
            lineString = line;
            explicitMatrixLine = split(lineString, " ");
            matrixAddLine(matrixTmp , explicitMatrixLine , stamp);
        }
        if(stamp != 0 && stamp % 4 ==0 ){
            pair<string , Matrix >  aPair(nameTmp , matrixTmp);
            matrix_string.insert(aPair);
            nameTmp = "~" + nameTmp;
            matrixTmp = ~matrixTmp;
            matrix_string.insert(pair<string , Matrix> (nameTmp , matrixTmp));
        }
        stamp++;
    }
    in.close();
    return matrix_string;
}

/*在读取matrix.in问价时读取到相应矩阵行数时，将该行加入到矩阵中去*/
void matrixAddLine(Matrix& matrix , vector<string> explicitMatrixLine , int stamp){
    vector<Bignum> bignums;
    for(int i = 0 ; i < explicitMatrixLine.size() ; i++){
        Bignum bignum;
        bignum = bignum.sToBignum(explicitMatrixLine.at(i));
        bignums.push_back(bignum);
    }
    for(int i = 0  ; i < 3 ; i++){
        matrix.setBignum(3 * ((stamp + 2 ) % 4 )+ i , bignums.at(i));
    }
}



//实现字符串的分割函数
vector<string> split(string s, string separator){
    vector<string> result;
    typedef string::size_type string_size;
    string_size i = 0;

    while(i != s.size()){
//        找到字符串中首个不等于分隔符的字母；
        int flag = 0;
        while(i != s.size() && flag == 0){
            flag = 1;
            for(char x : separator)
                if(s[i] == x){
                    ++i;
                    flag = 0;
                    break;
                }
        }
//        找到又一个分隔符，将两个分隔符之间的字符串取出；
        flag = 0;
        string_size j = i;
        while(j != s.size() && flag == 0){
            for(char x : separator)
                if(s[j] == x){
                    flag = 1;
                    break;
                }
            if(flag == 0)
                ++j;
        }
        if(i != j){
            result.push_back(s.substr(i, j-i));
            i = j;
        }
    }
    return result;
}

/*字符串用多个分割符分割*/
vector<string> split(string s, vector<string> seperatos){
    vector<string> result ;
    string tmp = s;
    for(string seperator: seperatos){
        result = split(tmp , seperator);
        tmp = toString(result);
    }
    return result;
}

/*将vector转化为string*/
string toString(vector<string> strings){
    string result;
    for(string string1:strings){
        result.append(string1);
    }
    return result;
}