#include "Vector.h"
#include "Expression.h"
#include "Function.h"
#include <iostream>
#include <map>
#include "test.h"
#include "Matrix.h"

using namespace std;

bool process(const string & cmd, int i);
void processExpression(const string & cmd, int index);
map <string, Vector *> vectorSet;
map <string, Function *> functionSet;
map <string, Matrix *> matrixSet;

MODE mode;

void insertBuiltInFunctions();

int main()
{
    cout << "***** Simple Calculator *****" << endl
         << "***** version 3.0 *****" << endl;
    //cout << "Please enter the mode label:\n" << "1. Sequence Storage\n"
    //     << "2. Chain Storage\n";
    string modeChoice;
    //getline(cin, modeChoice);
    modeChoice = "2";
    if(modeChoice == "1")
    {
        mode = SEQ;
    }
    else if(modeChoice == "2")
    {
        mode = CHAIN;
    }
    else
    {
        cout << "Invalid Choice. Exiting...\n";
        return 1;
    }

    insertBuiltInFunctions();

    for(int i = 0; ; i++)
    {
        string input;
        cout << "Input[" << i << "]:\n";
        getline(cin, input);

        if(!process(input, i))
        {
            cout << "Quitting...\n";
            break;
        }
    }

    return 0;
}

bool process(const string & cmd, int index)
{
    stringstream ss(cmd);
    string operatorName;
    ss >> operatorName;

    if(operatorName == "NEW")
    {
        string name;
        ss >> name;
        int dim;
        ss >> dim;
        auto temp = new Vector(mode, dim);
        string definition;
        getline(cin, definition);
        temp->define(definition);

        if(vectorSet.contains(name))
        {
            cout << "ERROR: Redefined symbol \"" << name << "\"" << endl;
            delete temp;
        }
        else
        {
            vectorSet[name] = temp;
            cout << "Output[" << index << "]:\n";
            cout << name << ":=";
            temp->show();
            cout << endl;
        }
    }
    else if(operatorName == "SHOW")
    {
        string name;
        ss >> name;

        if(vectorSet.contains(name))
        {
            cout << "Output[" << index << "]:\n";
            cout << name;
            if(vectorSet[name]->type == POLY)
                cout << "(x)";
            cout << "=";
            vectorSet[name]->show();
            cout << endl;
        }
        else if(matrixSet.contains(name))
        {
            cout << "Output[" << index << "]:\n";
            cout << name << "=\n";
            matrixSet[name]->print();
            cout << endl;
        }
        else
        {
            cout << "ERROR: Undefined symbol \"" << name << "\"\n";
        }
    }
    else if(operatorName == "DELETE")
    {
        string name;
        ss >> name;

        if(!vectorSet.contains(name))
        {
            cout << "ERROR: Undefined symbol \"" << name << "\"\n";
        }
        else
        {
            cout << "Output[" << index << "]:\n";
            cout << "Symbol \"" << name << "\" is deleted.\n";
            auto temp = vectorSet[name];
            vectorSet.erase(name);
            delete temp;
        }
    }
    else if(operatorName == "ADD")
    {
        string name1, name2;
        ss >> name1;
        ss >> name2;

        if(!vectorSet.contains(name1))
        {
            cout << "ERROR: Undefined symbol \"" << name1 << "\"\n";
        }
        else if(!vectorSet.contains(name2))
        {
            cout << "ERROR: Undefined symbol \"" << name2 << "\"\n";
        }
        else
        {
            auto v1 = vectorSet[name1];
            auto v2 = vectorSet[name2];

            if(v1->dimension != v2->dimension)
            {
                cout << "ERROR: Undefined operation between different dimension vectors\n";
            }
            else
            {
                auto result = new Vector(*v1 + *v2);

                string name3;
                ss >> name3;
                if (!name3.empty())
                {
                    if(vectorSet.contains(name3))
                    {
                        cout << "ERROR: Redefined symbol \"" << name3 << "\"\n";
                    }
                    else
                    {
                        cout << "Output[" << index << "]:\n";
                        cout << name3 << ":=";
                        vectorSet[name3] = result;
                    }
                }
                result->show();
                cout << endl;

                if(!vectorSet.contains(name3))
                    delete result;
            }
        }
    }
    else if(operatorName == "SUB")
    {
        string name1, name2;
        ss >> name1;
        ss >> name2;

        if(!vectorSet.contains(name1))
        {
            cout << "ERROR: Undefined symbol \"" << name1 << "\"\n";
        }
        else if(!vectorSet.contains(name2))
        {
            cout << "ERROR: Undefined symbol \"" << name2 << "\"\n";
        }
        else
        {
            auto v1 = vectorSet[name1];
            auto v2 = vectorSet[name2];

            if(v1->dimension != v2->dimension)
            {
                cout << "ERROR: Undefined operation between different dimension vectors\n";
            }
            else
            {
                auto result = new Vector(*v1 - *v2);

                string name3;
                ss >> name3;
                if (!name3.empty())
                {
                    if(vectorSet.contains(name3))
                    {
                        cout << "ERROR: Redefined symbol \"" << name3 << "\"\n";
                    }
                    else
                    {
                        cout << "Output[" << index << "]:\n";
                        cout << name3 << ":=";
                        vectorSet[name3] = result;
                    }
                }
                result->show();
                cout << endl;

                if(!vectorSet.contains(name3))
                    delete result;
            }
        }
    }
    else if(operatorName == "COS")
    {
        string name1, name2;
        ss >> name1;
        ss >> name2;

        if(!vectorSet.contains(name1))
        {
            cout << "ERROR: Undefined symbol \"" << name1 << "\"\n";
        }
        else if(!vectorSet.contains(name2))
        {
            cout << "ERROR: Undefined symbol \"" << name2 << "\"\n";
        }
        else
        {
            auto v1 = vectorSet[name1];
            auto v2 = vectorSet[name2];

            if(v1->dimension != v2->dimension)
            {
                cout << "ERROR: Undefined operation between different dimension vectors\n";
            }
            else
            {
                cout << "Output[" << index << "]:\n";
                cout << "Cos <" << name1 << ", " << name2 << "> = " << v1->cos(*v2) << endl;
            }
        }
    }
    else if(operatorName == "NEWPOLY")
    {
        string name;
        ss >> name;
        int dim;
        ss >> dim;
        auto temp = new Vector(mode, dim + 1, POLY);
        string definition;
        getline(cin, definition);
        temp->define(definition);

        if(vectorSet.contains(name))
        {
            cout << "ERROR: Redefined symbol \"" << name << "\"" << endl;
            delete temp;
        }
        else
        {
            vectorSet[name] = temp;
            cout << "Output[" << index << "]:\n";
            cout << name;
            if(temp->type == POLY)
                cout << "(x)";
            cout << ":=";
            temp->show();
            cout << endl;
        }
    }
    else if(operatorName == "ADDPOLY")
    {
        string name1, name2;
        ss >> name1;
        ss >> name2;

        if (!vectorSet.contains(name1))
        {
            cout << "ERROR: Undefined symbol \"" << name1 << "\"\n";
        }
        else if (!vectorSet.contains(name2))
        {
            cout << "ERROR: Undefined symbol \"" << name2 << "\"\n";
        }
        else
        {
            auto v1 = vectorSet[name1];
            auto v2 = vectorSet[name2];

            auto result = new Vector(*v1 + *v2);

            string name3;
            ss >> name3;
            if (!name3.empty())
            {
                if (vectorSet.contains(name3))
                {
                    cout << "ERROR: Redefined symbol \"" << name3 << "\"\n";
                }
                else
                {
                    cout << "Output[" << index << "]:\n";
                    cout << name3 << ":=";
                    vectorSet[name3] = result;
                }
            }
            result->show();
            cout << endl;

            if (!vectorSet.contains(name3))
                delete result;
        }
    }
    else if(operatorName == "SUBPOLY")
    {
        string name1, name2;
        ss >> name1;
        ss >> name2;

        if (!vectorSet.contains(name1))
        {
            cout << "ERROR: Undefined symbol \"" << name1 << "\"\n";
        }
        else if (!vectorSet.contains(name2))
        {
            cout << "ERROR: Undefined symbol \"" << name2 << "\"\n";
        }
        else
        {
            auto v1 = vectorSet[name1];
            auto v2 = vectorSet[name2];

            auto result = new Vector(*v1 - *v2);

            string name3;
            ss >> name3;
            if (!name3.empty())
            {
                if (vectorSet.contains(name3))
                {
                    cout << "ERROR: Redefined symbol \"" << name3 << "\"\n";
                }
                else
                {
                    cout << "Output[" << index << "]:\n";
                    cout << name3 << ":=";
                    vectorSet[name3] = result;
                }
            }
            result->show();
            cout << endl;

            if (!vectorSet.contains(name3))
                delete result;
        }
    }
    else if(operatorName == "MULTIPOLY")
    {
        string name1, name2;
        ss >> name1;
        ss >> name2;

        if (!vectorSet.contains(name1))
        {
            cout << "ERROR: Undefined symbol \"" << name1 << "\"\n";
        }
        else if (!vectorSet.contains(name2))
        {
            cout << "ERROR: Undefined symbol \"" << name2 << "\"\n";
        }
        else
        {
            auto v1 = vectorSet[name1];
            auto v2 = vectorSet[name2];

            auto result = new Vector(*v1 * *v2);

            string name3;
            ss >> name3;
            if (!name3.empty())
            {
                if (vectorSet.contains(name3))
                {
                    cout << "ERROR: Redefined symbol \"" << name3 << "\"\n";
                }
                else
                {
                    cout << "Output[" << index << "]:\n";
                    cout << name3 << ":=";
                    vectorSet[name3] = result;
                }
            }
            result->show();
            cout << endl;

            if (!vectorSet.contains(name3))
                delete result;
        }
    }
    else if(operatorName == "DSOLVE")
    {
        string name;
        ss >> name;
        int degree;
        ss >> degree;

        if (!vectorSet.contains(name))
        {
            cout << "ERROR: Undefined symbol \"" << name << "\"\n";
        }
        else if(degree < 1)
        {
            cout << "ERROR: Undefined derivative degree (" << degree << ")\n";
        }
        else
        {
            auto v = vectorSet[name];

            auto result = new Vector(v->derivative(degree));

            string resultName;
            ss >> resultName;
            if (!resultName.empty())
            {
                if (vectorSet.contains(resultName))
                {
                    cout << "ERROR: Redefined symbol \"" << resultName << "\"\n";
                }
                else
                {
                    cout << "Output[" << index << "]:\n";
                    cout << resultName << ":=";
                    vectorSet[resultName] = result;
                }
            }
            result->show();
            cout << endl;

            if (!vectorSet.contains(resultName))
                delete result;
        }
    }
    else if(operatorName == "SHOWALL")
    {
        cout << "Output[" << index << "]:\n";
        for(const auto& each: vectorSet)
        {
            string name = each.first;

            cout << name;
            if(vectorSet[name]->type == POLY)
                cout << "(x)";
            cout << "=";
            vectorSet[name]->show();
            cout << endl;
        }
        for(const auto & each: matrixSet)
        {
            string name = each.first;

            cout << name << "=\n";
            matrixSet[name]->print();
            cout << endl;
        }
    }
    else if(operatorName == "QUIT")
    {
        cout << "OK. ";
        return false;
    }
    else if(operatorName == "TEST")
    {
        int total = sizeof(QUESTION) / sizeof(QUESTION[0]);
        int correct = 0;
        for(int i = 0; i < total; i++)
        {
            cout << "Testing Point #" << i << ": ";
            cout << QUESTION[i] << endl;
            Expression testPoint(QUESTION[i]);
            if(fabs(testPoint.value.value - ANSWER[i]) < 1e-5)
            {
                cout << "Correct! " << "Answer is " << ANSWER[i] << endl;
                correct++;
            }
            else
            {
                cout << "Incorrect! Answer is  " << ANSWER[i] << ", gives " << testPoint.value << endl;
            }
            cout << endl;
        }
        cout << "Total Testing Point: " << total << endl;
        cout << "Correct Testing Point: " << correct << endl;
        cout << endl;
    }
    else if(operatorName == "LET")
    {
        string define = cmd.substr(4);

        // Delete the spaces
        ss.str("");
        for(int i = 0; i < define.length(); ++i)
        {
            char each = define[i];
            if(each != ' ')
                ss << each;
        }
        getline(ss, define);

        if(define.empty())
        {
            cout << "ERROR: Empty definition.\n";
            return true;
        }

        // First find the name and the function
        int firstEqual = define.find_first_of('=');
        int funcNameStart = -1, funcNameEnd = -1;
        map <int, string> variables;
        int varCount = 0;
        int tempStart = -1;
        string funcName, funcDefinition;

        for(int i = 0; i < firstEqual; i++)
        {
            if(funcNameStart == -1)
            {
                if(isalpha(define[i]) or define[i] == '_')
                {
                    funcNameStart = i;
                    i--;
                }
            }
            else if(funcNameEnd == -1)
            {
                if(define[i+1] == '(')
                    funcNameEnd = i;
            }
            else
            {
                if(tempStart == -1)
                {
                    if(define[i-1] == '(' or define[i-1] == ',')
                    {
                        tempStart = i;
                        i--;
                    }
                }
                else
                {
                    if(define[i+1] == ',' or define[i+1] == ')')
                    {
                        string varName = define.substr(tempStart, i - tempStart + 1);
                        varCount++;
                        variables[varCount] = varName;
                        tempStart = -1;
                    }
                }
            }
        }

        funcName = define.substr(funcNameStart, funcNameEnd - funcNameStart + 1);
        funcDefinition = define.substr(firstEqual + 1);

        for(const auto& each: variables)
        {
            string name = each.second;
            string target = string("x") + toStr(each.first);
            int temp;
            while((temp = findVarName(funcDefinition, name, target)) != string::npos)
            {
                strSubstitute(funcDefinition, temp, temp + name.length() - 1, target);
            }
        }

#ifdef DEBUG
        cout << funcName << "=" << funcDefinition << endl;
#endif

        if(functionSet.contains(funcName))
        {
            cout << "ERROR: Redefined function " << funcName << endl;
            return true;
        }
        else
        {
            cout << "Output[" << index << "]:\n";
            cout << funcName << ":=" << funcDefinition << endl;
            functionSet[funcName] = new Function(funcDefinition, varCount);
        }
    }
    else if(operatorName == "CLEAR")
    {
        string name;
        ss >> name;

        if(!functionSet.contains(name))
        {
            cout << "ERROR: Undefined function \"" << name << "\"\n";
        }
        else
        {
            cout << "Output[" << index << "]:\n";
            cout << "Function \"" << name << "\" is cleared.\n";
            auto temp = functionSet[name];
            functionSet.erase(name);
            delete temp;
        }
    }
    else if(operatorName == "NEWMAT")
    {
        string name;
        int row, col, ttemp = 0;
        ss >> name >> row;
        col = row;
        ss >> ttemp;
        if(ttemp > 0)
            col = ttemp;
        if(matrixSet.contains(name))
        {
            cout << "ERROR: Redefined matrix.\n";
            return true;
        }

        if(row <= 0 or col <= 0)
        {
            cout << "ERROR: Invalid row/column." << endl;
            return true;
        }

        auto temp = new Matrix(row, col);
        cout << "Define here:\n";
        temp->define();
        while(cin.get() != '\n');

        matrixSet[name] = temp;

        cout << "Output[" << index << "]:\n" << name << ":=\n";
        temp->print();
        cout << endl;
    }
    else if(operatorName == "DET")
    {
        string name;
        ss >> name;
        if(!matrixSet.contains(name))
        {
            cout << "ERROR: Undefined matrix." << endl;
            return true;
        }

        auto temp = matrixSet[name];

        if(!temp->isSquare)
        {
            cout << "ERROR: The matrix is not a squared matrix.\n";
            return true;
        }

        cout << "Output[" << index << "]:\n" << Determinant(*temp).calc() << endl;
    }
    else if(operatorName == "DELMAT")
    {
        string name;
        ss >> name;
        if(!matrixSet.contains(name))
        {
            cout << "ERROR: Undefined matrix.";
            return true;
        }

        auto temp = matrixSet[name];
        matrixSet.erase(name);
        delete temp;

        cout << "Output[" << index << "]:\n" << "Successfully deleted matrix " << name << endl;
    }
    else if(operatorName == "ADDMAT")
    {
        string name1, name2;
        ss >> name1;
        ss >> name2;

        if (!matrixSet.contains(name1))
        {
            cout << "ERROR: Undefined matrix \"" << name1 << "\"\n";
        }
        else if (!matrixSet.contains(name2))
        {
            cout << "ERROR: Undefined matrix \"" << name2 << "\"\n";
        }
        else
        {
            auto v1 = matrixSet[name1];
            auto v2 = matrixSet[name2];

            if(v1->rowSize != v2->rowSize or v1->colSize != v2->colSize)
            {
                cout << "ERROR: Invalid operation between different order matrix\n";
                return true;
            }

            auto result = new Matrix(*v1 + *v2);

            string name3;
            ss >> name3;
            if (!name3.empty())
            {
                if (matrixSet.contains(name3))
                {
                    cout << "ERROR: Redefined symbol \"" << name3 << "\"\n";
                }
                else
                {
                    cout << "Output[" << index << "]:\n";
                    cout << name3 << ":=\n";
                    matrixSet[name3] = result;
                }
            }
            result->print();
            cout << endl;

            if (!matrixSet.contains(name3))
                delete result;
        }
    }
    else if(operatorName == "SUBMAT")
    {
        string name1, name2;
        ss >> name1;
        ss >> name2;

        if (!matrixSet.contains(name1))
        {
            cout << "ERROR: Undefined matrix \"" << name1 << "\"\n";
        }
        else if (!matrixSet.contains(name2))
        {
            cout << "ERROR: Undefined matrix \"" << name2 << "\"\n";
        }
        else
        {
            auto v1 = matrixSet[name1];
            auto v2 = matrixSet[name2];

            if(v1->rowSize != v2->rowSize or v1->colSize != v2->colSize)
            {
                cout << "ERROR: Invalid operation between different order matrix\n";
                return true;
            }

            auto result = new Matrix(*v1 - *v2);

            string name3;
            ss >> name3;
            if (!name3.empty())
            {
                if (matrixSet.contains(name3))
                {
                    cout << "ERROR: Redefined symbol \"" << name3 << "\"\n";
                }
                else
                {
                    cout << "Output[" << index << "]:\n";
                    cout << name3 << ":=\n";
                    matrixSet[name3] = result;
                }
            }
            result->print();
            cout << endl;

            if (!matrixSet.contains(name3))
                delete result;
        }
    }
    else if(operatorName == "MULTIMAT")
    {
        string name1, name2;
        ss >> name1;
        ss >> name2;

        if (!matrixSet.contains(name1))
        {
            cout << "ERROR: Undefined matrix \"" << name1 << "\"\n";
        }
        else if (!matrixSet.contains(name2))
        {
            cout << "ERROR: Undefined matrix \"" << name2 << "\"\n";
        }
        else
        {
            auto v1 = matrixSet[name1];
            auto v2 = matrixSet[name2];

            if(v1->colSize != v2->rowSize)
            {
                cout << "ERROR: Invalid operation between selected matrix\n";
                return true;
            }

            auto result = new Matrix(*v1 * *v2);

            string name3;
            ss >> name3;
            if (!name3.empty())
            {
                if (matrixSet.contains(name3))
                {
                    cout << "ERROR: Redefined symbol \"" << name3 << "\"\n";
                }
                else
                {
                    cout << "Output[" << index << "]:\n";
                    cout << name3 << ":=\n";
                    matrixSet[name3] = result;
                }
            }
            result->print();
            cout << endl;

            if (!matrixSet.contains(name3))
                delete result;
        }
    }
    else
    {
        // cout << "ERROR: Undefined command \"" << operatorName << "\"\n";
        // Solve the arithmetic expression
        processExpression(cmd, index);
    }

    return true;
}

void processExpression(const string & cmd, int index)
{
    Expression input(cmd);
    cout << "Output[" << index << "]:\n";
    cout << input.value << endl;
}

void insertBuiltInFunctions()
{
    // If need to add a built-in function, there are 3 steps:
    // First, here
    // Second, in Expression.calc() method
    // Third, in a const array called BUILT_IN_FUNCTIONS in Function.h

    // TODO
    // Add a tag in Function class to identify built-in property

    functionSet[string("sin")] = new Function("sin(x1)", 1);
    functionSet[string("cos")] = new Function("cos(x1)", 1);
    functionSet[string("tan")] = new Function("tan(x1)", 1);
    functionSet[string("sinh")] = new Function("sinh(x1)", 1);
    functionSet[string("cosh")] = new Function("cosh(x1)", 1);
    functionSet[string("tanh")] = new Function("tanh(x1)", 1);
    functionSet[string("exp")] = new Function("exp(x1)", 1);
    functionSet[string("ln")] = new Function("ln(x1)", 1);
    functionSet[string("log")] = new Function("log(x1,x2)", 2);
    functionSet[string("ceiling")] = new Function("ceiling(x1)", 1);
    functionSet[string("floor")] = new Function("floor(x1)", 1);
    functionSet[string("round")] = new Function("round(x1)", 1);
    functionSet[string("asin")] = new Function("asin(x1)", 1);
    functionSet[string("acos")] = new Function("acos(x1)", 1);
    functionSet[string("atan")] = new Function("atan(x1)", 1);
    functionSet[string("precision")] = new Function("precision(x1)", 1); // 设置输出结果有效位数
}