//
// Created by 石依凡 on 2021/10/16.
//

#include "Expression.h"
#include <set>
#include "Function.h"

using namespace std;

extern std::map<std::string, Function*> functionSet;

int getPriority(const string & operatorName)
{
    // Notice
    // # as the beginning is 0
    // +, - used as add and sub is 1
    // *, / is 2
    // +, - used as positive and negative is 3
    // ^ used as power is 4
    // (, ) is 5

    // Here only consider +, - as add, sub
    if(operatorName == "#")
        return 0;
    else if(operatorName == "+" or operatorName == "-")
        return 1;
    else if(operatorName == "*" or operatorName == "/")
        return 2;
    else if(operatorName == "^")
        return 4;
    else if(operatorName == "(" or operatorName == ")")
        return 5;
    else
        return -1;
}

bool isOperator(char a)
{
    if(a == '+' or a == '-' or a == '*' or a == '/' or a == '(' or a == ')' or a == '^')
        return true;
    else
        return false;
}

bool isOperator(const string & a)
{
    if(a == "+" or a == "-" or a == "*" or a == "/" or a == "(" or a == ")" or a == "^")
        return true;
    else
        return false;
}

Expression::Expression(const std::string & _expr): expr(_expr), value()
{
    originalExpr = expr;
    value = calc();
}

Value Expression::calc()
{
    // First, judge if there is a SYNTAX_ERROR
    value.status = getStatus();
    if(!OK() and value.status != VOID)
    {
        return value;
    }

    // Delete spaces in input
    stringstream ss;
    for(auto each: expr)
    {
        if(each != ' ')
            ss << each;
    }
    getline(ss, expr);

    // Then, do the variable substitute
    if(expr.find('|') != string::npos)
    {
        // Judge if valid
        for(int i = expr.find('|') + 1; i < expr.length(); i++)
        {
            if(!(isalpha(expr[i]) or isNum(expr[i]) or expr[i] == ',' or expr[i] == '=' or expr[i] == '^' or expr[i]
            == '_'))
            {
                value.status = SYNTAX_ERROR;
                return value;
            }
        }

        ss.clear();
        int current = expr.find('|') + 1;
        map <string, ResultType> variables;
        bool loopFlag = true;

        // Get variables list
        while(loopFlag)
        {
            // Variable names should ONLY contain alphabet and numbers
            int varStart = -1, varEnd = -1;
            int valueStart = -1, valueEnd = -1;

            for( ; current < expr.length(); current++)
            {
                if(varStart == -1)
                {
                    if(isalpha(expr[current]) or isdigit(expr[current]) or expr[current] == '_')
                    {
                        if(!(isalpha(expr[current - 1]) or isdigit(expr[current - 1])))
                        {
                            varStart = current;
                            current--;
                        }
                    }
                }
                else if(varEnd == -1)
                {
                    if(isalpha(expr[current]) or isdigit(expr[current]))
                    {
                        if(expr[current + 1] == '=')
                            varEnd = current;
                    }
                }
                else if(valueStart == -1)
                {
                    if(isNum(expr[current]))
                    {
                        if(expr[current - 1] == '=')
                        {
                            valueStart = current;
                            current--;
                        }
                    }
                }
                else if(valueEnd == -1)
                {
                    if(isNum(expr[current]) or tolower(expr[current]) == 'e')
                    {
                        if(current == expr.length() - 1 or !(isNum(expr[current+1]) or tolower(expr[current+1]) ==
                        'e'))
                        {
                            valueEnd = current;
                            current++;
                            break;
                        }
                    }
                }
            }

            if(varStart == -1 and varEnd == -1 and valueStart == -1 and valueEnd == -1)
                loopFlag = false;

            if(loopFlag and (varStart == -1 or varEnd == -1 or valueStart == -1 or valueEnd == -1))
            {
                value.status = SYNTAX_ERROR;
                return value;
            }

            if(loopFlag)
            {
                string varName = expr.substr(varStart, varEnd - varStart + 1);
                string valueName = expr.substr(valueStart, valueEnd - valueStart + 1);
                if(variables.contains(varName))
                {
                    value.status = SYNTAX_ERROR;
                    return value;
                }
                else if(!isValidVarName(varName))
                {
                    value.status = SYNTAX_ERROR;
                    return value;
                }
                else
                {
                    variables[varName] = toNum(valueName);
                }
            }
        }

        int equalCounts = 0;
        for(auto each: expr)
        {
            if(each == '=')
                equalCounts++;
        }
        if(variables.size() != equalCounts)
        {
            value.status = SYNTAX_ERROR;
            return value;
        }

        // Substitute
        for(auto eachVar: variables)
        {
            while(findVarName(expr, eachVar.first) != string::npos)
            {
                int start = findVarName(expr, eachVar.first);
                strSubstitute(expr, start, start + eachVar.first.length() - 1, eachVar.second);
            }
        }
        expr.erase(expr.find('|'));

#ifdef DEBUG
        cout << expr << endl;
#endif
    }

    // Calculate all the functions
    int tempFuncNameStart = -1, tempFuncNameEnd = -1;
    int fromTemp = 0;
    while(true)
    {
        findFunc(expr, tempFuncNameStart, tempFuncNameEnd, fromTemp);
        if(tempFuncNameEnd == -1)
            break;

        // Check the database
        string funcName = expr.substr(tempFuncNameStart, tempFuncNameEnd - tempFuncNameStart + 1);
        if(!functionSet.contains(funcName))
        {
            cout << "ERROR: Undefined function " << funcName << endl;
            value.status = SYNTAX_ERROR;
            return value;
        }
        Function * func = functionSet[funcName];

        // Find how many variables
        int tempLeft = tempFuncNameEnd + 1;
        int tempRight = findOppositeBracket(expr, tempLeft);
        int varNum = 1;
        int commaPos[100];
        for(int i = 0; i < 100; i++)
            commaPos[i] = -2;

        for(int i = tempLeft + 1; i < tempRight; i++)
        {
            if(expr[i] == '(')
            {
                i = findOppositeBracket(expr, i);
                continue;
            }
            if(expr[i] == ',')
            {
                commaPos[varNum-1] = i;
                varNum++;
            }
        }

        // Check varNum
        if(varNum != func->varNum)
        {
            cout << "ERROR: Wrong number of variables\n";
            value.status = SYNTAX_ERROR;
            return value;
        }

        // Calculate each part
        Expression * eachPart = new Expression[varNum];
        int eachPartStart = tempLeft + 1;
        int eachPartEnd = varNum == 1 ? tempRight - 1 : commaPos[0] - 1;
        eachPart[0] = expr.substr(eachPartStart, eachPartEnd - eachPartStart + 1);
        for(int i = 1; i < varNum; i++)
        {
            if(i != varNum - 1)
            {
                eachPartStart = commaPos[i - 1] + 1;
                eachPartEnd = commaPos[i] - 1;
            }
            else
            {
                eachPartStart = commaPos[i - 1] + 1;
                eachPartEnd = tempRight - 1;
            }
            eachPart[i].expr = expr.substr(eachPartStart, eachPartEnd - eachPartStart + 1);
        }

        Value tempValue;
        tempValue.status = DONE;

        if(isBuiltIn(funcName))
        {
            switch(varNum)
            {
                case 1:
                {
                    Value xtemp = eachPart[0].calc();
                    if(xtemp.status != DONE)
                        return xtemp;
                    ResultType x = xtemp.value;
                    double (*f) (double) = sin;

                    if(funcName == "sin")
                        f = sin;
                    else if(funcName == "cos")
                        f = cos;
                    else if(funcName == "tan")
                    {
                        if(fabs((x / 3.14159 * 2) - round(x / 3.14159 * 2)) < 1e-7)
                        {
                            tempValue.status = MATH_ERROR;
                            return tempValue;
                        }
                        f = tan;
                    }
                    else if(funcName == "sinh")
                        f = sinh;
                    else if(funcName == "cosh")
                        f = cosh;
                    else if(funcName == "tanh")
                        f = tanh;
                    else if(funcName == "exp")
                        f = exp;
                    else if(funcName == "ln")
                    {
                        if(x <= 0)
                        {
                            tempValue.status = MATH_ERROR;
                            return tempValue;
                        }
                        f = log;
                    }
                    else if(funcName == "ceiling")
                        f = ceil;
                    else if(funcName == "floor")
                        f = floor;
                    else if(funcName == "round")
                        f = round;
                    else if(funcName == "asin")
                    {
                        if(fabs(x) > 1)
                        {
                            tempValue.status = MATH_ERROR;
                            return tempValue;
                        }
                        f = asin;
                    }
                    else if(funcName == "acos")
                    {
                        if(fabs(x) > 1)
                        {
                            tempValue.status = MATH_ERROR;
                            return tempValue;
                        }
                        f = acos;
                    }
                    else if(funcName == "atan")
                        f = atan;
                    else if(funcName == "precision")
                    {
                        tempValue.value = round(x);
                        cout.precision(round(x));
                        break;
                    }

                    tempValue.value = f(x);
                }
                    break;
                case 2:
                {
                    Value xtemp = eachPart[0].calc();
                    Value ytemp = eachPart[1].calc();
                    if(xtemp.status != DONE)
                        return xtemp;
                    if(ytemp.status != DONE)
                        return ytemp;
                    ResultType x = xtemp.value;
                    ResultType y = ytemp.value;

                    if(x <= 0 or y <= 0)
                    {
                        tempValue.status = MATH_ERROR;
                        return tempValue;
                    }

                    tempValue.value = log(y) / log(x);
                }
                    break;
                default:
                    break;
            }
        }
        else
        {
            tempValue = func->calc(eachPart, varNum);
        }

        delete [] eachPart;

        if(tempValue.status != DONE)
            return tempValue;

        strSubstitute(expr, tempFuncNameStart, tempRight, tempValue.value);
    }

    // Here, the expr should be a normal expression
    // Fix the original expression (without spaces and variables)
    originalExpr = expr;

    // Main arithmetic process
    Stack <Operator> operatorStack;
    operatorStack.push(Operator(string("#"), 0));
    Stack <ResultType> operandStack;

    if(expr.empty())
    {
        value.status = VOID;
        return value;
    }

    // Add spaces to isolate different parts
    // Need to add space: number and operators, non-positive/negative operators and numbers
    // Also: brackets and others, operators and positive/negative operators
    // Also: positive/negative operators and non-numbers
    ss.clear();
    for(int i = 0; i < expr.length(); ++i)
    {
        ss << expr[i];
        if(i < expr.length() - 1 and (expr[i+1] == '(' or expr[i+1] == ')'))
            ss << " ";
        else if(isdigit(expr[i]) and i < expr.length() - 1)
        {
            if(isOperator(expr[i+1]))
            {
                ss << " ";
            }
        }
        else if(isOperator(expr[i]))
        {
            if(expr[i] == '*' or expr[i] == '/')
            {
                ss << " ";
            }
            else if(expr[i] == '+' or expr[i] == '-')
            {
                // If is non-positive/negative operator
                if(!(i == 0 or (isOperator(expr[i - 1]) and expr[i - 1] != ')')))
                    ss << " ";
                else if(i < expr.length() - 1 and !isdigit(expr[i+1]) and expr[i+1] != '.')
                    ss << " ";
            }
            else
            {
                ss << " ";
            }
        }
    }
    getline(ss, expr);

#ifdef DEBUG
    cout << expr << endl;
#endif

    // Start to read
    ss.clear();
    for(auto each: expr)
        ss << each;

    string temp;
    int current = -1;   // Mark the tail of current position
    // Reading each element of expression
    while(ss >> temp)
    {
        if(temp.empty())
            break;

        current += temp.length();
        if(isOperator(temp))
        {
            // If is negative or positive operator
            if(isSpecial(current))
            {
                while(operatorStack.top().priority >= 3)
                {
                    ResultType origin = operandStack.top();
                    if(!operandStack.pop())
                    {
                        value.status = SYNTAX_ERROR;
                        return value;
                    }

                    auto originOp = operatorStack.top();
                    operatorStack.pop();
                    if(originOp.name == "-")
                        origin = - origin;
                    operandStack.push(origin);
                }
                operatorStack.push(Operator(temp, 3));
            }
            // If is not special but is right bracket
            else if(temp == ")")
            {
                while(operatorStack.top().name != "(")
                {
                    auto topOp = operatorStack.top();
                    bool flag = operatorStack.pop();
                    if(!flag)
                    {
                        value.status = SYNTAX_ERROR;
                        return value;
                    }

                    if(topOp.priority == 3)
                    {
                        ResultType origin = operandStack.top();
                        bool flag1 = operandStack.pop();
                        if(topOp.name == "-")
                            origin = -origin;
                        operandStack.push(origin);
                    }
                    else
                    {
                        ResultType latter = operandStack.top();
                        bool flag1 = operandStack.pop();
                        ResultType former = operandStack.top();
                        bool flag2 = operandStack.pop();

                        if(!flag1 or !flag2)
                        {
                            value.status = SYNTAX_ERROR;
                            return value;
                        }

                        if(topOp.name == "/" and fabs(latter) < 1e-7)
                        {
                            value.status = MATH_ERROR;
                            return value;
                        }

                        ResultType tempResult = simpleCalc(topOp.name, former, latter);
                        operandStack.push(tempResult);
                    }
                }
                operatorStack.pop();
            }
            // If is a left bracket
            else if(temp == "(")
            {
                operatorStack.push(Operator(temp, 0));
            }
            // If is a normal operator
            else
            {
                int currentPriority = getPriority(temp);
                while(operatorStack.top().priority >= currentPriority and operatorStack.top().name != "#")
                {
                    auto tempOp = operatorStack.top();
                    operatorStack.pop();

                    if(tempOp.priority == 3)
                    {
                        if(tempOp.name == "-")
                        {
                            ResultType origin = operandStack.top();
                            bool ok = operandStack.pop();
                            if(!ok)
                            {
                                value.status = SYNTAX_ERROR;
                                return value;
                            }
                            operandStack.push(-origin);
                        }
                    }
                    else
                    {

                        ResultType latter = operandStack.top();
                        bool flag1 = operandStack.pop();
                        ResultType former = operandStack.top();
                        bool flag2 = operandStack.pop();

                        if (!flag1 or !flag2)
                        {
                            value.status = SYNTAX_ERROR;
                            return value;
                        }

                        if (temp == "/" and fabs(latter) < 1e-7)
                        {
                            value.status = MATH_ERROR;
                            return value;
                        }

                        ResultType tempResult = simpleCalc(tempOp.name, former, latter);
                        operandStack.push(tempResult);
                    }
                }
                operatorStack.push(Operator(temp, getPriority(temp)));
            }
        }
        // If is operand
        else
        {
            operandStack.push(toNum(temp));
        }
    }

    // Clear the stacks to get the final result
    while(operatorStack.top().name != "#")
    {
        Operator tempOp = operatorStack.top();
        bool flag = operatorStack.pop();
        if(!flag)
        {
            value.status = SYNTAX_ERROR;
            return value;
        }

        if(tempOp.priority == 3)
        {
            ResultType origin = operandStack.top();
            bool flag1 = operandStack.pop();
            if(tempOp.name == "-")
                origin = -origin;
            operandStack.push(origin);
        }
        else
        {
            ResultType latter = operandStack.top();
            bool flag1 = operandStack.pop();
            ResultType former = operandStack.top();
            bool flag2 = operandStack.pop();

            if(!flag1 or !flag2)
            {
                value.status = SYNTAX_ERROR;
                return value;
            }

            if(tempOp.name == "/" and fabs(latter) < 1e-7)
            {
                value.status = MATH_ERROR;
                return value;
            }

            ResultType tempResult = simpleCalc(tempOp.name, former, latter);
            operandStack.push(tempResult);
        }
    }

    if(operandStack.size == 1)
    {
        value.status = DONE;
        value.value = operandStack.top();
    }
    else
    {
        value.status = SYNTAX_ERROR;
        return value;
    }
    return value;
}

CalcStatus Expression::getStatus()
{
    // If value.status has been defined
    if(value.status != VOID)
        return value.status;

    // If not
    // Check for undefined symbols
    set <char> legalOther = {'.', ',', ' ', '+', '-', '*', '/', '(', ')', '|', '=', ',', '^', '_'};
    for(auto each: expr)
    {
        if(!isalpha(each) and !isdigit(each) and !legalOther.contains(each))
        {
            value.status = SYNTAX_ERROR;
            return SYNTAX_ERROR;
        }
    }

    // Check the proper usage of brackets
    // Seems unuseful when dealing with functions
    /*
    int left_bracket = 0, right_bracket = 0;
    for(int i = 0; i < expr.length(); i++)
    {
        if(expr[i] == '(')
        {
            if(i > 0 and (isdigit(expr[i - 1]) or expr[i - 1] == '.'))
            {
                value.status = SYNTAX_ERROR;
                return SYNTAX_ERROR;
            }
            left_bracket++;
        }
        else if(expr[i] == ')')
        {
            if(i < expr.length() - 1 and (isdigit(expr[i + 1]) or expr[i + 1] == '.'))
            {
                value.status = SYNTAX_ERROR;
                return SYNTAX_ERROR;
            }
            right_bracket++;
        }

        if(right_bracket > left_bracket)
        {
            value.status = SYNTAX_ERROR;
            return SYNTAX_ERROR;
        }
    }
    if(left_bracket != right_bracket)
    {
        value.status = SYNTAX_ERROR;
        return SYNTAX_ERROR;
    }
     */

    return VOID;
}

bool Expression::OK() const
{
    // If value.status is not OK
    if(value.status != DONE)
        return false;

    return true;
}

bool Expression::isSpecial(int i)
{
    if(originalExpr[i] == '+' or originalExpr[i] == '-')
    {
        if(i == 0)
            return true;
        if(originalExpr[i-1] == '(')
            return true;
        if(originalExpr[i-1] != ')' and !isdigit(originalExpr[i-1]))
            return true;
    }

    return false;
}

Operator::Operator(const std::string& _name, int _priority): name(_name), priority(_priority) {}

ResultType toNum(const std::string & a)
{
    stringstream ss(a);
    ResultType r;
    ss >> r;

    return r;
}

ResultType simpleCalc(const std::string & op, ResultType former, ResultType latter)
{
    if(op == "+")
        return former + latter;
    if(op == "-")
        return former - latter;
    if(op == "*")
        return former * latter;
    if(op == "/")
        return former / latter;
    if(op == "^")
        return pow(former, latter);

    return 0;
}

void strSubstitute(std::string & origin, int from, int end, ResultType target)
{
    stringstream ss;
    for(int i = 0; i < origin.length(); i++)
    {
        if(i != from)
        {
            ss << origin[i];
        }
        else
        {
            ss << "(" << target << ")";
            i = end;
        }
    }
    getline(ss, origin);
}

void strSubstitute(std::string & origin, int from, int end, const std::string & target)
{
    stringstream ss;
    for(int i = 0; i < origin.length(); i++)
    {
        if(i != from)
        {
            ss << origin[i];
        }
        else
        {
            ss << target;
            i = end;
        }
    }
    getline(ss, origin);
}

std::ostream& operator<<(std::ostream & os, Value value)
{
    if(value.status == DONE)
    {
        os << value.value;
    }
    else
    {
        if(value.status == SYNTAX_ERROR)
            cout << "SYNTAX ERROR!";
        else if(value.status == MATH_ERROR)
            cout << "MATH ERROR!";
        else
            cout << "NOTHING!";
    }

    return os;
}

bool isValidVarName(string var)
{
    if(isdigit(var[0]))
        return false;

    for(const auto & each: var)
    {
        if(!isalpha(each) and !isdigit(each) and each != '_')
            return false;
    }
    return true;
}