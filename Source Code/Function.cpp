//
// Created by 石依凡 on 2021/10/23.
//

#include "Function.h"

using namespace std;

Function::Function(const std::string &definition, int _varNum)
{
    expr = definition;
    varNum = _varNum;
}

string Function::convert(ResultType *source, int length) const
{
    stringstream ss;
    ss << '|';
    for(int i = 0; i < length; i++)
    {
        if(i != 0)
            ss << ',';
        ss << "x" << i+1 << "=" << source[i];
    }
    string expression;
    getline(ss, expression);
    string result = expr + expression;
    return result;
}

Value Function::calc(ResultType *source, int varNum) const
{
    Expression result(convert(source, varNum));
    return result.value;
}

Value Function::calc(Expression *source, int varNum) const
{
    ResultType * temp = new ResultType[varNum];
    Value ttemp;
    for(int i = 0; i < varNum; i++)
    {
        ttemp = source[i].calc();
        if(ttemp.status != DONE)
            return ttemp;
        temp[i] = ttemp.value;
    }

    Value result = calc(temp, varNum);

    delete [] temp;

    return result;
}

std::string toStr(int from)
{
    stringstream ss;
    ss << from;
    string temp;
    getline(ss, temp);
    return temp;
}

string::size_type findVarName(const std::string & mother, const std::string & name, const std::string & avoid, int from)
{
    string::size_type result = from;
    int size = name.length();

    while((result = mother.find(name, result)) != string::npos)
    {
        if(mother.find(avoid, result) == result)
        {
            result++;
        }
        else
        {
            if(result + size >= mother.length())
            {
                if (isalpha(mother[result - 1]) or isdigit(mother[result - 1]) or mother[result - 1] == '_')
                    result++;
                else
                    return result;
            }
            else
            {
                if (isalpha(mother[result - 1]) or isalpha(mother[result + size]) or isdigit(mother[result - 1]) or
                    isdigit(mother[result + size]) or mother[result - 1] == '_' or mother[result + size] == '_' or
                    mother[result + size] == '(')
                    result++;
                else
                    return result;
            }
        }
    }

    return string::npos;
}

void findFunc(const std::string & expr, int & funcNameStart, int & funcNameEnd, int from)
{
    if(expr.empty())
        return;

    funcNameStart = -1;
    funcNameEnd = -1;

    for(int i = from; i < expr.length() - 1; ++i)
    {
        if((isalpha(expr[i]) or isdigit(expr[i]) or expr[i] == '_') and expr[i+1] == '(')
        {
            funcNameEnd = i;
            break;
        }
    }

    if(funcNameEnd != -1)
    {
        for(int i = funcNameEnd; i >= from; i--)
        {
            if(((isalpha(expr[i]) or expr[i] == '_') and !(isalpha(expr[i-1]) or expr[i-1] == '_')) or i == from)
            {
                funcNameStart = i;
                break;
            }
        }
    }
}

int findOppositeBracket(const std::string & expr, int bracket)
{
    int current = 1;

    if(expr[bracket] == '(')
    {
        for(int i = bracket+1; i < expr.length(); i++)
        {
            if(expr[i] == '(')
                current++;
            else if(expr[i] == ')')
                current--;
            if(current == 0)
                return i;
        }
    }
    else if(expr[bracket] == ')')
    {
        for(int i = bracket-1; i >= 0; i--)
        {
            if(expr[i] == ')')
                current++;
            else if(expr[i] == '(')
                current--;
            if(current == 0)
                return i;
        }
    }

    return -1;
}