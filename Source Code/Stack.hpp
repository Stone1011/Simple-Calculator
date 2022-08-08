//
// Created by 石依凡 on 2021/10/16.
//

#ifndef SIMPLE_CALCULATOR_2_0_STACK_HPP
#define SIMPLE_CALCULATOR_2_0_STACK_HPP

template <typename T = double>
class ChainNode
{
public:
    T data;
    ChainNode <T> * before;
    ChainNode <T> (): data(T()), before(nullptr) {};
};

template <typename T = double>
class Stack
{
public:
    // Data Members
    ChainNode <T> * headPointer, * topPointer;
    int size;

    // Constructors
    Stack();

    // Destructors
    ~Stack();

    // Methods
    bool push(T one);
    bool pop();
    T& top() const;
    bool isEmpty() const;
};

template<typename T>
Stack<T>::Stack(): size(0)
{
    headPointer = new ChainNode <T>;
    topPointer = headPointer;
}

template<typename T>
bool Stack<T>::isEmpty() const
{
    return topPointer == headPointer;
}

template<typename T>
T &Stack<T>::top() const
{
    return topPointer->data;
}

template<typename T>
bool Stack<T>::pop()
{
    if(!isEmpty())
    {
        auto temp = topPointer;
        topPointer = topPointer->before;
        delete temp;
        size--;

        return true;
    }

    return false;
}

template<typename T>
bool Stack<T>::push(T one)
{
    auto * temp = new ChainNode <T>;
    if(temp == nullptr)
        return false;

    temp->before = topPointer;
    temp->data = one;
    topPointer = temp;

    size++;
    return true;
}

template<typename T>
Stack<T>::~Stack()
{
    while(topPointer != nullptr)
    {
        auto temp = topPointer->before;
        delete topPointer;
        topPointer = temp;
    }
}

#endif //SIMPLE_CALCULATOR_2_0_STACK_HPP
