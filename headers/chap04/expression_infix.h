#pragma once

#include "stack.h"
#include "expression_element.h"
#include "expression_framework.h"

// 这个文件实现了中缀表达式
// 1. 解析中缀表达式的字符串
// 2. 对中缀表达式进行计算

namespace clazy {

// 实现了中缀表达式
class InfixExpression : public clazy_framework::AbstractExpression {
private:
    Vector<ExpressionElement> V;
public:
    void parse(string expr) override;
    int getResult() const override;

    void print() const override {
        cout << V << endl;
    }

    void clear() override {
        V.clear();
    }

    friend class SuffixExpression; // 后缀表达式那里需要实现中缀转后缀的功能
};

// 字符串转换为中缀表达式
// 1. 开头和结尾强制加了一对括号，以减少计算中的特判
// 2. 如果遇到负号，则将负号替换为@，以减少计算中的特判
void InfixExpression::parse(string expr) {
    clear();
    V.push_back('(');                               // 在表达式的开头加上一个括号
    int numberBuffer = 0;                           // 用来存放正在读取的数字
    bool isReadingNumber = false;                   // 表示是否正在读取数字
    for (char c : expr) {
        if (isdigit(c)) {                           // 如果发现是数字，则继续读取
            numberBuffer = numberBuffer * 10 + c - '0';
            isReadingNumber = true;
        } else {                                    // 如果发现是符号
            if (isReadingNumber) {                  // 符号前面有数字的情况，先把数字放到表达式的解析结果中
                V.back().setOperand(numberBuffer);
                numberBuffer = 0;
                isReadingNumber = false;
            } else {                                // 符号前面没有数字，这里进行一次特判
                if (c == '-' && V.back() == '(') {  // 并且符号前面是左括号，则判定-是负号
                    V.push_back('@');
                    continue;
                }
            }
            V.push_back(c);                         // 除了负号以外的情况，都直接放入表达式
        }
    }
    if (isReadingNumber) {                          // 以数字结尾的情况，把最后一个数字放入表达式
        V.back().setOperand(numberBuffer);
    }
    V.push_back(')');                               // 在表达式的结尾加上一个括号
}

// 中缀表达式的计算
// 这里展示的方法是不基于后缀表达式转换的
// 不过事实上，中缀表达式转换成后缀表达式，和这个方法如出一辙
int InfixExpression::getResult() const {
    Stack<ExpressionElement> S;
    S.push('(');                                 // 在栈开头加入一个空元素，保证栈非空
    for (auto& e : V) {
        while (S.top().prior(e)) {               // 当栈内的运算符优先级更高时，它需要先进行计算
            auto op = S.pop();                   // 取出栈内的运算符，执行运算并将结果压回栈中
            S.top().setOperand(op(S.top().getOperand(), op.getOperand()));
            if (op == '(') { break; }            // 如果是左括号（说明e是右括号）则跳出循环
        }
        if (e != ')') {                          // 如果e不是右括号则让它进栈
            S.push(e);
        }
    }
    return S.pop().getOperand();                 // 取出栈顶元素（一开始加进去的空元素），即为计算结果
}

}