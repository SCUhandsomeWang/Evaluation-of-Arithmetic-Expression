#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_
#include <cmath>
#include <iostream>
#include <string>
#include <stack>
#include <sstream>
#include <vector>
#include <cctype>
#include <stdexcept>
#include "lk_stack.h"
using namespace std;
template <class ElemType>
class Calculator {
private:
    LinkStack<char> optr;
    LinkStack<ElemType> opnd;
    int OperPriro(char op);
    void Get2Operands( ElemType &left, ElemType &right);
    ElemType Operate(ElemType left, char op, ElemType right);
    bool IsOperator(char ch);

public:
    Calculator();
    ~Calculator();
    void Run();
    
};




template <class ElemType>
inline int Calculator<ElemType>::OperPriro(char op)
{
    if(op =='=') return 1;
    if(op =='('||op ==')') return 2;
    if(op =='+'||op =='-') return 3;
    if(op =='*'||op =='/') return 4;
    if(op =='^') return 5;
    return 0;
}

template <class ElemType>
void Calculator<ElemType>::Get2Operands(ElemType &left, ElemType &right)
{
    opnd.Top(right);
    opnd.Pop();
    opnd.Top(left);
    opnd.Pop();
}

template <class ElemType>
ElemType Calculator<ElemType>::Operate(ElemType left, char op, ElemType right)
{
    ElemType res;
    switch(op)
    {
        case '+':
            res = left + right;
            break;
        case '-':
            res = left - right;
            break;
        case '*':
            res = left * right;
            break;
        case '/':
            res = left / right;
            break;
        case '^':
            res = std::pow(left, right);
            break;
        default:
            throw std::invalid_argument("Invalid operator");
    }
    return res;
}

template <class ElemType>
bool Calculator<ElemType>::IsOperator(char ch)
{
    if(ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '(' || ch == ')' || ch == '=')
        return true;
    return false;
}

template <class ElemType>
void Calculator<ElemType>::Run()
{
    optr.Clear();
    opnd.Clear();
    cout << "Please input the infix arithmetic expression, ending with =:" << endl;

    string expression;
    getline(cin >> ws, expression);
    size_t end = expression.find('=');
    if (end == string::npos)
        throw invalid_argument("Expression must end with =");
    expression.erase(end);

    vector<string> postfix;
    optr.Push('=');
    bool expectOperand = true;

    for (size_t i = 0; i < expression.length();)
    {
        char ch = expression[i];
        if (isspace(static_cast<unsigned char>(ch)))
        {
            ++i;
            continue;
        }

        if (isdigit(static_cast<unsigned char>(ch)) || ch == '.')
        {
            if (!expectOperand)
                throw invalid_argument("Invalid expression: missing operator");

            size_t start = i;
            bool hasDigit = false;
            while (i < expression.length() &&
                   (isdigit(static_cast<unsigned char>(expression[i])) ||
                    expression[i] == '.'))
            {
                hasDigit = hasDigit ||
                    isdigit(static_cast<unsigned char>(expression[i]));
                ++i;
            }
            if (!hasDigit)
                throw invalid_argument("Invalid number");

            postfix.push_back(expression.substr(start, i - start));
            expectOperand = false;
            continue;
        }

        if (!IsOperator(ch) || ch == '=')
            throw invalid_argument("Invalid character in expression");

        if ((ch == '+' || ch == '-') && expectOperand)
            postfix.push_back("0");
        else if (expectOperand && ch != '(')
            throw invalid_argument("Invalid expression: operator position");

        if (ch == '(')
        {
            if (!expectOperand)
                throw invalid_argument("Invalid expression: missing operator");
            optr.Push(ch);
            expectOperand = true;
        }
        else if (ch == ')')
        {
            if (expectOperand)
                throw invalid_argument("Invalid expression: unexpected ')'");

            char top;
            bool foundLeftParenthesis = false;
            while (optr.Top(top) && top != '=')
            {
                optr.Pop(top);
                if (top == '(')
                {
                    foundLeftParenthesis = true;
                    break;
                }
                postfix.push_back(string(1, top));
            }
            if (!foundLeftParenthesis)
                throw invalid_argument("Invalid expression: mismatched parentheses");
            expectOperand = false;
        }
        else
        {
            char top;
            while (optr.Top(top) && top != '=' && top != '(' &&
                   (OperPriro(top) > OperPriro(ch) ||
                    (OperPriro(top) == OperPriro(ch) && ch != '^')))
            {
                optr.Pop(top);
                postfix.push_back(string(1, top));
            }
            optr.Push(ch);
            expectOperand = true;
        }
        ++i;
    }

    if (expectOperand)
        throw invalid_argument("Invalid expression: missing operand");

    char top;
    while (optr.Top(top) && top != '=')
    {
        optr.Pop(top);
        if (top == '(' || top == ')')
            throw invalid_argument("Invalid expression: mismatched parentheses");
        postfix.push_back(string(1, top));
    }

    cout << "postfix: ";
    for (size_t i = 0; i < postfix.size(); ++i)
        cout << postfix[i] << (i + 1 == postfix.size() ? '\n' : ' ');

    for (const string &token : postfix)
    {
        if (token.length() > 1 ||
            !IsOperator(token[0]) || token[0] == '(' || token[0] == ')')
        {
            stringstream stream(token);
            ElemType number;
            if (!(stream >> number) || !stream.eof())
                throw invalid_argument("Invalid number");
            opnd.Push(number);
        }
        else
        {
            ElemType left, right;
            if (!opnd.Pop(right) || !opnd.Pop(left))
                throw invalid_argument("Invalid expression: insufficient operands");
            opnd.Push(Operate(left, token[0], right));
        }
    }

    ElemType answer;
    if (!opnd.Pop(answer) || !opnd.IsEmpty())
        throw invalid_argument("Invalid expression");
    cout << "answer:" << answer << std::endl;
}

template <class ElemType>
Calculator<ElemType>::Calculator()
{
}

template <class ElemType>
Calculator<ElemType>::~Calculator()
{
}

#endif // _CALCULATOR_H_