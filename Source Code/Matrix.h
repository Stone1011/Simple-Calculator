//
// Created by 石依凡 on 2021/10/24.
//

#ifndef SIMPLE_CALCULATOR_3_0_MATRIX_H
#define SIMPLE_CALCULATOR_3_0_MATRIX_H

#include "Expression.h"

class Matrix
{
public:
    // Data Members
    double ** data;
    int rowSize, colSize;
    bool isSquare;

    // Constructors
    Matrix() = default;
    Matrix(int row, int col); // Assign new spaces for the matrix, and set all the elements to zero
    Matrix(const Matrix & other);

    // Destructor
    ~Matrix();

    // Operators
    Matrix & operator=(const Matrix & other);
    Matrix operator+(const Matrix & other);
    Matrix operator-(const Matrix & other);
    Matrix operator*(const Matrix & other);

    // Print
    void print();

    // Define from keyboard
    void define();
};

class Determinant: public Matrix
{
public:
    Value value = Value();

    using Matrix::Matrix;
    Determinant(const Matrix & other);
    ~Determinant();

    Value calc();
    Determinant cofactor(int i, int j);
};

#endif //SIMPLE_CALCULATOR_3_0_MATRIX_H
