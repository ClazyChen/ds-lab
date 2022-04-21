#pragma once

#include "expression_infix.h"

// 这个文件实现了后缀表达式

namespace clazy {

class SuffixExpression : public clazy_framework::AbstractExpression {
private:
    Vector<ExpressionElement> V;

    // 中缀表达式转后缀表达式
    void infix2suffix(const InfixExpression& infix);
public:
    void parse(string expr) override {
        InfixExpression infix;
        infix.parse(expr);
        infix2suffix(infix);
    }
    int getResult() const override;

    void clear() override {
        V.clear();
    }

    friend ostream& operator<< (ostream& out, const SuffixExpression& expr);
    friend class ExpressionTree;   // 表达式树需要从后缀表达式转
};

// 中缀表达式转换为后缀表达式
// 这个过程和中缀表达式的计算具有高度的形式一致性
// 这从侧面反映了，后缀表达式是一种适合计算的表达式
// 实际上，这里的栈可以只存char，但由于prior被封装在ExpressionElement中，这里存的是封装后的类型
void SuffixExpression::infix2suffix(const InfixExpression& infix) {
    Stack<ExpressionElement> S;
    S.push('(');                                 // 在栈开头加入一个空元素，保证栈非空
    for (auto& e : infix.V) {
        while (S.top().prior(e)) {               // 当栈内的运算符优先级更高时，它需要先进行计算
            auto op = S.pop();                   // 取出栈内的运算符，执行运算并将结果压回栈中
            if (op == '(') { break; }            // 如果是左括号（说明e是右括号）则跳出循环
            V.push_back(op.getOperator());       // 将运算符压入后缀表达式
        }
        if (e != ')') {                          // 如果e不是右括号则让它进栈
            S.push(e);
        }
        if (e.hasOperand()) {                    // 如果包含操作数，将操作数加入到后缀表达式
            V.push_back(e.getOperand());
        }
    }
}

// 计算后缀表达式
int SuffixExpression::getResult() const {
    Stack<int> S;
    for (auto& e : V) {
        if (e.hasOperand()) {
            S.push(e.getOperand());
        } else {
            auto [l, r] = e.operandPosition();
            int right = r ? S.pop() : 0;
            int left  = l ? S.pop() : 0;
            S.push(e(left, right));
        }
    }
    return S.pop();
}

// 输出后缀表达式
ostream& operator<< (ostream& out, const SuffixExpression& expr) {
    out << expr.V;
    return out;
}


}