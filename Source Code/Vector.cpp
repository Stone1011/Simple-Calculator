//
// Created by 石依凡 on 2021/9/30.
//

#include "Vector.h"
#include <iostream>
#include <sstream>
#include <cmath>

using namespace std;

Vector::Vector(MODE _mode, int _dimension, TYPE _type): type(_type)
{
    mode = _mode;
    dimension = _dimension;

    if(mode == SEQ)
    {
        data = new DataType[dimension];
    }
    else
    {
        head = new Node;
        auto current = head;
        for(int i = 1; i < dimension; i++)
        {
            current->next = new Node;
            current = current->next;
        }
    }
}

Vector::Vector(const Vector &origin): mode(origin.mode), dimension(origin.dimension), type(origin.type)
{
    if (mode == SEQ)
    {
        data = new DataType[dimension];
        for(int i = 0; i < dimension; i++)
        {
            data[i] = origin.data[i];
        }
    }
    else
    {
        head = new Node;
        auto current = head;
        auto currentOrigin = origin.head;

        while(currentOrigin != nullptr)
        {
            current->data = currentOrigin->data;
            if(currentOrigin->next != nullptr)
            {
                current->next = new Node;
                current = current->next;
            }
            currentOrigin = currentOrigin->next;
        }
    }
}

Vector::~Vector()
{
    if(mode == SEQ)
    {
        delete [] data;
    }
    else
    {
        auto current = head;
        while(current != nullptr)
        {
            head = head->next;
            delete current;
            current = head;
        }
    }
}

void Vector::define(const string & definition) const
{
    stringstream ss(definition);

    Node * current = head;
    for(int i = 0; i < dimension; i++)
    {
        DataType temp;
        ss >> temp;

        if(mode == SEQ)
        {
            data[i] = temp;
        }
        else
        {
            current->data = temp;
            if(i != dimension - 1)
            {
                current = current->next;
            }
        }
    }
}

void Vector::show() const
{
    if(type == VECTOR)
    {
        cout << "[";
        Node *current = head;
        for (int i = 0; i < dimension; i++)
        {
            if (mode == SEQ)
            {
                cout << data[i];
            }
            else
            {
                cout << current->data;
                current = current->next;
            }

            if (i != dimension - 1)
            {
                cout << ", ";
            }
        }
        cout << "]";
    }
    else
    {
        Node *current = head;
        bool allZero = true;
        for (int i = 0; i < dimension; i++)
        {
            if (mode == SEQ)
            {
                if(i != 0 and data[i] > GAP_TO_ZERO)
                    cout << "+";
                if(fabs(data[i]) > GAP_TO_ZERO)
                {
                    cout << data[i];
                    allZero = false;
                    if (i != 0)
                        cout << "x";
                    if (i > 1)
                        cout << "^" << i;
                }
            }
            else
            {
                if(i != 0 and current->data > GAP_TO_ZERO)
                    cout << "+";
                if(fabs(current->data) > GAP_TO_ZERO)
                {
                    cout << current->data;
                    allZero = false;
                    if (i != 0)
                        cout << "x";
                    if (i > 1)
                        cout << "^" << i;
                }
                current = current->next;
            }
        }
        if(allZero)
            cout << 0;
    }
}

Vector Vector::operator+(const Vector & another) const
{
    if(type == another.type && type == VECTOR)
    {
        Vector result(mode, dimension);
        string definition;
        stringstream ss;

        if (mode == SEQ)
        {
            for (int i = 0; i < dimension; i++)
            {
                ss << data[i] + another.data[i] << " ";
            }
        }
        else
        {
            auto current1 = head;
            auto current2 = another.head;
            for (int i = 0; i < dimension; i++)
            {
                ss << current1->data + current2->data << " ";
                current1 = current1->next;
                current2 = current2->next;
            }
        }

        getline(ss, definition);
        result.define(definition);
        return result;
    }
    else if(type == another.type && type == POLY)
    {
        Vector result(mode, max(dimension, another.dimension), POLY);
        string definition;
        stringstream ss;

        if(mode == SEQ)
        {
            int minDimension = min(dimension, another.dimension);
            int maxDimension = max(dimension, another.dimension);

            for(int i = 0; i < minDimension; i++)
            {
                ss << data[i] + another.data[i] << " ";
            }

            const Vector & large = dimension > another.dimension ? *this : another;

            for(int i = minDimension; i < maxDimension; i++)
            {
                ss << large.data[i] << " ";
            }
        }
        else
        {
            int minDimension = min(dimension, another.dimension);
            int maxDimension = max(dimension, another.dimension);

            auto current1 = head;
            auto current2 = another.head;

            for (int i = 0; i < minDimension; i++)
            {
                ss << current1->data + current2->data << " ";
                current1 = current1->next;
                current2 = current2->next;
            }

            Node * large = dimension > another.dimension ? current1 : current2;
            for(int i = minDimension; i < maxDimension; i++)
            {
                ss << large->data << " ";
                large = large->next;
            }
        }

        getline(ss, definition);
        result.define(definition);
        return result;
    }
    else
    {
        Vector result(mode, 1);
        if(result.mode == SEQ)
            result.data[0] = 0;
        else
            result.head->data = 0;
        return result;
    }
}

Vector Vector::operator-(const Vector &another) const
{
    if(type == another.type && type == VECTOR)
    {
        Vector result(mode, dimension);
        string definition;
        stringstream ss;

        if (mode == SEQ)
        {
            for (int i = 0; i < dimension; i++)
            {
                ss << data[i] - another.data[i] << " ";
            }
        }
        else
        {
            auto current1 = head;
            auto current2 = another.head;
            for (int i = 0; i < dimension; i++)
            {
                ss << current1->data - current2->data << " ";
                current1 = current1->next;
                current2 = current2->next;
            }
        }

        getline(ss, definition);
        result.define(definition);
        return result;
    }
    else if(type == another.type && type == POLY)
    {
        Vector result(mode, max(dimension, another.dimension), POLY);
        string definition;
        stringstream ss;

        if(mode == SEQ)
        {
            int minDimension = min(dimension, another.dimension);
            int maxDimension = max(dimension, another.dimension);

            for(int i = 0; i < minDimension; i++)
            {
                ss << data[i] - another.data[i] << " ";
            }

            const Vector & large = dimension > another.dimension ? *this : another;
            DataType flag = dimension > another.dimension ? 1 : -1;

            for(int i = minDimension; i < maxDimension; i++)
            {
                ss << flag * large.data[i] << " ";
            }
        }
        else
        {
            int minDimension = min(dimension, another.dimension);
            int maxDimension = max(dimension, another.dimension);

            auto current1 = head;
            auto current2 = another.head;

            for (int i = 0; i < minDimension; i++)
            {
                ss << current1->data - current2->data << " ";
                current1 = current1->next;
                current2 = current2->next;
            }

            Node * large = dimension > another.dimension ? current1 : current2;
            DataType flag = dimension > another.dimension ? 1 : -1;
            for(int i = minDimension; i < maxDimension; i++)
            {
                ss << flag * large->data << " ";
                large = large->next;
            }
        }

        getline(ss, definition);
        result.define(definition);
        return result;
    }
    else
    {
        Vector result(mode, 1);
        if(result.mode == SEQ)
            result.data[0] = 0;
        else
            result.head->data = 0;
        return result;
    }
}

double Vector::cos(const Vector &other) const
{
    double dotResult = 0;
    double len = 0;
    double otherLen = 0;

    if(mode == SEQ)
    {
        for(int i = 0; i < dimension; i++)
        {
            dotResult += data[i] * other.data[i];
            len += data[i] * data[i];
            otherLen += other.data[i] * other.data[i];
        }
    }
    else
    {
        auto current1 = head;
        auto current2 = other.head;

        for(int i = 0; i < dimension; i++)
        {
            dotResult += current1->data * current2->data;
            len += current1->data * current1->data;
            otherLen += current2->data * current2->data;
            current1 = current1->next;
            current2 = current2->next;
        }
    }

    len = sqrt(len);
    otherLen = sqrt(otherLen);
    double result = dotResult / len / otherLen;
    return result;
}

Vector Vector::multiNum(DataType num, int degree) const
{
    Vector result(mode, dimension + degree, type);
    string definition;
    stringstream ss;

    for(int i = 0; i < degree; i++)
    {
        ss << 0 << " ";
    }

    if(mode == SEQ)
    {
        for(int i = degree; i < degree + dimension; i++)
        {
            ss << num * data[i - degree] << " ";
        }
    }
    else
    {
        auto current = head;
        for(int i = degree; i < degree + dimension; i++)
        {
            ss << current->data * num << " ";
            current = current->next;
        }
    }

    getline(ss, definition);
    result.define(definition);
    return result;
}

Vector Vector::operator*(const Vector &other) const
{
    Vector result(mode, dimension + other.dimension, type);
    if(mode == SEQ)
    {
        for(int i = 0; i < other.dimension; i++)
        {
            Vector temp = multiNum(other.data[i], i);
            Vector temp2 = result + temp;
            result = temp2;
        }
    }
    else
    {
        auto current = other.head;
        for(int i = 0; i < other.dimension; i++)
        {
            Vector temp = multiNum(current->data, i);
            Vector temp2 = result + temp;
            result = temp2;
            current = current->next;
        }
    }

    return result;
}

Vector& Vector::operator=(const Vector &origin)
{
    type = origin.type;
    mode = origin.mode;
    dimension = origin.dimension;

    if(mode == SEQ)
    {
        delete [] data;
    }
    else
    {
        auto current = head;
        while(current != nullptr)
        {
            head = head->next;
            delete current;
            current = head;
        }
    }

    if (mode == SEQ)
    {
        data = new DataType[dimension];
        for(int i = 0; i < dimension; i++)
        {
            data[i] = origin.data[i];
        }
    }
    else
    {
        head = new Node;
        auto current = head;
        auto currentOrigin = origin.head;

        while(currentOrigin != nullptr)
        {
            current->data = currentOrigin->data;
            if(currentOrigin->next != nullptr)
            {
                current->next = new Node;
                current = current->next;
            }
            currentOrigin = currentOrigin->next;
        }
    }

    return *this;
}

Vector Vector::derivative(int degree) const
{
    if(degree == 1)
    {
        Vector result(mode, max(dimension - 1, 0), type);
        string definition;
        stringstream ss;

        if (mode == SEQ)
        {
            for (int i = 1; i < dimension; i++)
            {
                ss << (DataType) i * data[i] << " ";
            }
        }
        else
        {
            auto current = head->next;
            for (int i = 1; i < dimension; i++)
            {
                ss << (DataType) i * current->data << " ";
                current = current->next;
            }
        }

        getline(ss, definition);
        result.define(definition);
        return result;
    }
    else
        return derivative(degree-1).derivative();
}

