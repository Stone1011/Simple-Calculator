//
// Created by 石依凡 on 2021/10/16.
//

#ifndef SIMPLE_CALCULATOR_2_0_EXPRESSION_H
#define SIMPLE_CALCULATOR_2_0_EXPRESSION_H

#include <string>
#include <cstring>
#include <cmath>
#include <iostream>
#include <map>
#include "Stack.hpp"
#include <sstream>

//#define DEBUG

typedef double ResultType;

enum CalcStatus {VOID, DONE, SYNTAX_ERROR, MATH_ERROR};

bool isOperator(char a);

bool isOperator(const std::string & a);

int getPriority(const std::string & operatorName);

ResultType toNum(const std::string & a);

ResultType simpleCalc(const std::string & op, ResultType former, ResultType latter);

void strSubstitute(std::string & origin, int from, int end, ResultType target);

void strSubstitute(std::string & origin, int from, int end, const std::string & target);

class Operator
{
public:
    std::string name;
    int priority;

    Operator() = default;
    Operator(const std::string& _name, int _priority);
};

struct Value
{
    ResultType value;
    CalcStatus status;

    Value(): value(0), status(VOID){}
};

std::ostream& operator<<(std::ostream & os, Value);

class Expression
{
public:
    // Data Elements
    // Expression
    std::string expr, originalExpr;
    Value value;

    // Constructors
    Expression() = default;
    Expression(const std::string & _expr);

    // Destructors
    ~Expression() = default;

    // Calculation
    Value calc();

    // Get Status
    CalcStatus getStatus();

    // Total status OK
    bool OK() const;

    // Judge if expr[i] is negative or positive operator
    bool isSpecial(int i);
};

inline bool isNum(char a)
{
    return isdigit(a) or a == '-' or a == '+' or a == '.';
}

bool isValidVarName(std::string var);

#endif //SIMPLE_CALCULATOR_2_0_EXPRESSION_H
