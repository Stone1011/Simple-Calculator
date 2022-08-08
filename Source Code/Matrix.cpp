//
// Created by 石依凡 on 2021/10/24.
//

#include "Matrix.h"
#include <iostream>

using namespace std;

Matrix::Matrix(int row, int col): rowSize(row), colSize(col), isSquare(row == col)
{
    data = new double* [rowSize];
    for(int i = 0; i < rowSize; i++)
    {
        data[i] = new double[colSize];
    }

    for(int i = 0; i < rowSize; i++)
    {
        for(int j = 0; j < colSize; j++)
        {
            data[i][j] = 0;
        }
    }
}

Matrix::~Matrix()
{
    for(int i = 0; i < rowSize; i++)
        delete [] data[i];
    delete [] data;
}

Matrix::Matrix(const Matrix &other): rowSize(other.rowSize), colSize(other.colSize), isSquare(other.isSquare)
{
    data = new double* [rowSize];
    for(int i = 0; i < rowSize; i++)
    {
        data[i] = new double[colSize];
    }

    for(int i = 0; i < rowSize; i++)
    {
        for(int j = 0; j < colSize; j++)
        {
            data[i][j] = other.data[i][j];
        }
    }
}

Matrix &Matrix::operator=(const Matrix &other)
{
    for(int i = 0; i < rowSize; i++)
        delete [] data[i];
    delete [] data;

    rowSize = other.rowSize;
    colSize = other.colSize;
    isSquare = other.isSquare;

    data = new double* [rowSize];
    for(int i = 0; i < rowSize; i++)
    {
        data[i] = new double[colSize];
    }

    for(int i = 0; i < rowSize; i++)
    {
        for(int j = 0; j < colSize; j++)
        {
            data[i][j] = other.data[i][j];
        }
    }

    return *this;
}

void Matrix::print()
{
    for(int i = 0; i < rowSize; i++)
    {
        for(int j = 0; j < colSize; j++)
        {
            cout << data[i][j] << "\t";
        }
        cout << endl;
    }
}

void Matrix::define()
{
    for(int i = 0; i < rowSize; i++)
    {
        for(int j = 0; j < colSize; j++)
        {
            cin >> data[i][j];
        }
    }
}

Matrix Matrix::operator+(const Matrix &other)
{
    Matrix result(rowSize, colSize);
    for(int i = 0; i < rowSize; i++)
    {
        for(int j = 0; j < colSize; j++)
        {
            result.data[i][j] = data[i][j] + other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix &other)
{
    Matrix result(rowSize, colSize);
    for(int i = 0; i < rowSize; i++)
    {
        for(int j = 0; j < colSize; j++)
        {
            result.data[i][j] = data[i][j] - other.data[i][j];
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix &other)
{
    Matrix result(rowSize, other.colSize);

    for(int i = 0; i < rowSize; i++)
    {
        for(int j = 0; j < other.colSize; j++)
        {
            for(int k = 0; k < colSize; k++)
            {
                result.data[i][j] += data[i][k] * other.data[k][j];
            }
        }
    }

    return result;
}

Value Determinant::calc()
{
    if(!isSquare)
    {
        value.status = MATH_ERROR;
        return value;
    }

    value.status = DONE;

    if(rowSize == 2)
    {
        value.value = data[0][0] * data[1][1] - data[0][1] * data[1][0];
        return value;
    }

    ResultType result = 0;
    for(int i = 0; i < colSize; i++)
    {
        if(i % 2 == 0)
        {
            ResultType temp = data[0][i] * cofactor(0, i).calc().value;
            result += temp;
        }
        else
        {
            ResultType temp = data[0][i] * cofactor(0, i).calc().value;
            result -= temp;
        }
    }

    value.value = result;
    return value;
}

Determinant Determinant::cofactor(int i, int j)
{
    Determinant result(rowSize - 1, colSize - 1);

    int mIndex = 0, nIndex = 0;

    for(int m = 0; m < rowSize; m++)
    {
        nIndex = 0;
        if(m == i)
        {
            continue;
        }
        for(int n = 0; n < colSize; n++)
        {
            if(n == j)
            {
                continue;
            }
            result.data[mIndex][nIndex] = data[m][n];
            nIndex++;
        }
        mIndex++;
    }

    return result;
}

Determinant::~Determinant() {}

Determinant::Determinant(const Matrix &other) : Matrix(other)
{
    data = new double* [rowSize];
    for(int i = 0; i < rowSize; i++)
    {
        data[i] = new double[colSize];
    }

    for(int i = 0; i < rowSize; i++)
    {
        for(int j = 0; j < colSize; j++)
        {
            data[i][j] = other.data[i][j];
        }
    }
}
