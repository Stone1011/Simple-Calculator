cmake_minimum_required(VERSION 3.20)
project(Simple_Calculator_3_0)

set(CMAKE_CXX_STANDARD 23)

add_executable(Simple_Calculator_3_0 main.cpp Vector.cpp Vector.h Expression.cpp Expression.h Stack.hpp test.h Function.cpp Function.h Matrix.cpp Matrix.h)
