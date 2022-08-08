//
// Created by 石依凡 on 2021/10/17.
//

#ifndef SIMPLE_CALCULATOR_2_0_TEST_H
#define SIMPLE_CALCULATOR_2_0_TEST_H

#include <string>
#include "Expression.h"

const std::string QUESTION [] {
    "1+1",
    "-(-2)",
    "1-(-2)",
    "(-2)*(-4)",
    "(2)",
    "(3.14 - 3) / (2+1/2)",
    "-3*-3",
    "(x-1)*(x+3)|x=2",
    "(x1+1)*(x2+1)|x1=-2,x2=-4",
    "(x1+1)*(x2+1)|x1=-2,x2=3.14159",
    "2*(-x+2*y)|y=2,x=2"
};

const ResultType ANSWER [] {
    2,
    2,
    3,
    8,
    2,
    0.056,
    9,
    5,
    3,
    -4.14159,
    4
};

#endif //SIMPLE_CALCULATOR_2_0_TEST_H
