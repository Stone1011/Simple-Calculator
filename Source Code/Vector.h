//
// Created by 石依凡 on 2021/9/30.
//

#ifndef SIMPLE_CALCULATOR_1_0_VECTOR_H
#define SIMPLE_CALCULATOR_1_0_VECTOR_H

#include <string>

typedef double DataType;
const double GAP_TO_ZERO = 1e-6;

enum MODE {SEQ, CHAIN};
enum TYPE {VECTOR, POLY};

struct Node
{
    DataType data;
    Node * next;

    Node(): data(0), next(nullptr) {}
};

class Vector
{
public:
    // Sequence Storage, mode == 1
    // Chain Storage, mode == 2
    MODE mode;
    TYPE type;
    DataType * data;    // For SEQ
    Node * head;    // FOR CHAIN

    int dimension;

    // Constructor
    Vector() = default;
    explicit Vector(MODE _mode, int _dimension = 0, TYPE _type = VECTOR);
    // Destructor
    ~Vector();
    // Copying Constructor
    Vector(const Vector & origin);
    // Assignment operator
    Vector& operator=(const Vector & origin);

    // Define
    void define(const std::string & definition) const;
    // Show
    void show() const;

    // Add
    Vector operator+(const Vector& other)const;
    // Sub
    Vector operator-(const Vector& other)const;
    // Cos
    [[nodiscard]] double cos(const Vector& other)const;
    // Multiplied by a number with a degree
    [[nodiscard]] Vector multiNum(DataType num, int degree) const;
    // Multiplied by another Vector
    Vector operator*(const Vector& other) const;
    // Derivative
    Vector derivative(int degree = 1) const;

};


#endif //SIMPLE_CALCULATOR_1_0_VECTOR_H
