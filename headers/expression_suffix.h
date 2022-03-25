#pragma once

#include "expression_infix.h"

namespace clazy {

// 后缀表达式
// 继承了中缀表达式
class SuffixExpression : public LinearExpression {
protected:
    void infix2suffix(const InfixExpression& expr); // 中缀表达式转换为后缀表达式

public:
    virtual int getResult();
    virtual void parse(string expr) {
        removeElements();
        InfixExpression infixExpr(expr);
        infix2suffix(infixExpr);
    }

    SuffixExpression() {}
    SuffixExpression(string expr) { parse(expr); }

    friend ostream& operator<<(ostream& out, const SuffixExpression& expr);
};

// 中缀表达式转换为后缀表达式
// 这个过程和中缀表达式的计算具有高度的形式一致性
// 这从侧面反映了，后缀表达式是一种适合计算的表达式
// 为了和中缀表达式计算的过程保持一致，这里同样使用了智能指针
// 需要指出的是，智能指针和普通指针混用是一个很差的编程习惯
// 这里因为智能指针被限制在了函数体内部，所以影响不大
// 但如果开放的接口混用了智能指针和普通指针，很容易造成非法访存问题
// 自己写代码时应当尽量避免这种编程习惯
void SuffixExpression::infix2suffix(const InfixExpression& expr) {
    Stack<shared_ptr<MixedElement>> S; // 同样用栈，这里实际上只需要存运算符
    for (auto e : expr.V) {
        if (e->isOperator()) {
            while (!S.empty() && S.top()->higherPriority(*e)) {
                if (auto t = S.pop(); t->getOperator() == '(') {
                    if (S.empty()) { return; } // 由于中缀表达式在外面加了一堆括号，必定可以从这里返回
                    break;
                } else {
                    V.push_back(new Operator(t->getOperator()));
                }
            }
        }
        if (e->isOperand()) {
            V.push_back(new Operand(e->getOperand()));
        } else if (e->getOperator() != ')') {
            S.push(make_shared<MixedElement>(e));
        }
    }
}

// 后缀表达式的计算
int SuffixExpression::getResult() {
    Stack<int> S;                               // 计算使用的辅助栈，用来存放中间结果
    for (auto e : V) {
        if (e->isOperand()) {                   // 如果是操作数，直接压入栈
            S.push(e->getOperand());
        } else {
            auto [l, r] = e->operandPosition(); // 获取运算符的两个操作数的位置上是否存在操作数
            int right = r ? S.pop() : 0;        // 注意入栈顺序是反的
            int left = l ? S.pop() : 0;
            S.push(e->apply(left, right));
        }
    }
    return S.pop();
}

ostream& operator<<(ostream& out, const SuffixExpression& expr) {
    for (bool first = true; auto e : expr.V) {
        if (first) {
            first = false;
        } else {
            out << " ";
        }
        out << *e;
    }
    return out;
}


}