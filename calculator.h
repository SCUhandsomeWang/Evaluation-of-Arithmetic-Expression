#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_
#include <cmath>
#include <iostream>
#include <string>
#include <stack>
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
    optr.Push('=');
    char ch;
    char priorChar;
    char optrTop;
    cout << "Please input the infix arithmetic expression, ending with =:" << endl;
    cin >> ch;
    priorChar = ch;
    optr.Top(optrTop);
    while(ch != '=' || optrTop != '=')
    {
        optr.Top(optrTop);
        if(optrTop == '=' && ch == '=')
        {
            ElemType ans;
            opnd.Top(ans);
            cout << "answer:" << ans << std::endl;
            return;
        }
        if(!IsOperator(ch))
        {
            cin.putback(ch);
            ElemType num;
            cin >>num;
            opnd.Push(num);
            priorChar = 'n';
            cin >> ch;
        }
        else
        {
            if((ch =='+'||ch =='-') && (priorChar =='=' || priorChar =='('))
            {
                opnd.Push(0);
            }
            if((optrTop == ')'&& ch == '('))
            {
                throw invalid_argument("Invalid expression: mismatched parentheses");
            }
            if(optrTop=='(' && ch == ')')
            {
                optr.Pop();
                priorChar = ch;
                cin >> ch;
                continue;
            }
            if(ch == '('||OperPriro(optrTop) < OperPriro(ch))
            {
                optr.Push(ch);
                cin >> ch;
                priorChar = ch;
            }
            else
            {
                char op;
                optr.Pop(op);
                ElemType left, right;
                Get2Operands(left, right);
                ElemType res = Operate(left, op, right);
                opnd.Push(res);
            }
        }
    }
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