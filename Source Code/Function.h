//
// Created by 石依凡 on 2021/10/23.
//

#ifndef SIMPLE_CALCULATOR_3_0_FUNCTION_H
#define SIMPLE_CALCULATOR_3_0_FUNCTION_H

#include <string>
#include <map>
#include "Expression.h"

const std::string BUILT_IN_FUNCTIONS [] {
    "sin",
    "cos",
    "tan",
    "sinh",
    "cosh",
    "tanh",
    "exp",
    "ln",
    "log",
    "ceiling",
    "floor",
    "round",
    "asin",
    "acos",
    "atan",
    "precision"
};

inline bool isBuiltIn(std::string a)
{
    for(const auto & i : BUILT_IN_FUNCTIONS)
    {
        if(a == i)
            return true;
    }
    return false;
}

std::string toStr(int from);

class Function
{
public:
    // Data Member
    std::string expr;
    int varNum;

    // Constructor
    Function() = default;
    Function(const std::string & definition, int _varNum);

    //Solve the value
    std::string convert(ResultType * source, int varNum) const;
    Value calc(ResultType * source, int varNum) const;
    Value calc(Expression * source, int varNum) const;
};

std::string::size_type findVarName(const std::string & mother, const std::string & name, const std::string & avoid =
"!@#$%%^", int from = 0);

void findFunc(const std::string & expr, int & funcNameStart, int & funcNameEnd, int from = 0);

int findOppositeBracket(const std::string & expr, int bracket);

#endif //SIMPLE_CALCULATOR_3_0_FUNCTION_H
