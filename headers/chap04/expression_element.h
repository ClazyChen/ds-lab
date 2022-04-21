#pragma once

#include "expression_element_framework.h"
#include "power.h"
#include "factorial.h"

// 这个文件具体实现了一个表示表达式元素的类
// 在我的示例表达式计算程序里也使用了这个类
// 这里支持下面的运算符：
// 加、减、乘、除、求余、乘方、负号、阶乘
// 其中负号使用符号@表示，以避免与其他符号冲突
// 如果负号也使用-，那么这个框架就不能正常工作了，因为-号会被解释为减号
// 很容易构造出两个完全一样的后缀表达式，对应不同的中缀表达式，当负号和减号用同一个符号时
// 阶乘使用!表示，乘方使用^表示

namespace clazy {

class ExpressionElement : public clazy_framework::AbstractExpressionElement {
private:
    // 通用的乘方计算器、阶乘计算器    
    static Power power;
    static Factorial factorial;

    // 存储运算符和操作数
    char operator_ = '\0';
    int operand = 0;
    bool operand_valid = false;

public:
    char getOperator() const override { return operator_; }
    int getOperand() const override { return operand; }
    void setOperator(char operator_) override { this->operator_ = operator_; }
    void setOperand(int operand) override { this->operand = operand; operand_valid = true; }

    pair<bool, bool> operandPosition() const override {
        switch (this->getOperator()) {
            case '(':
            case '@': return {false, true};
            case ')': 
            case '!': return {true, false};
            case '+':
            case '-':
            case '*':
            case '/':
            case '%':
            case '^': return {true, true};
            default:  return {false, false};
        }
    }

    int apply(int left, int right) override {
        switch (this->getOperator()) {
            case '+': return left + right;
            case '-': return left - right;
            case '*': return left * right;
            case '/': return left / right;
            case '%': return left % right;
            case '^': return power(left, right);
            case '@': return - right;
            case '!': return factorial(left);
            default:  return this->getOperand();
        }
    }

    bool prior(const clazy_framework::AbstractExpressionElement& next) const override {
        auto getPriority = [](char c) -> pair<int, int> {  // 这里分别是，在左侧的优先级，在右侧的优先级
            switch (c) {                 // 即栈内优先级、栈外优先级
                case '(': return {0, 6}; // 左括号出现在右边的时候优先级最高，而出现在左边的时候优先级最低
                case ')': return {0, 0}; // 右括号永远不会进栈，优先级是最低的
                case '+': return {1, 1};
                case '-': return {1, 1};
                case '*': return {2, 2};
                case '/': return {2, 2};
                case '%': return {2, 2};
                case '^': return {3, 3};
                case '@': return {4, 4};
                case '!': return {5, 5};
                default:  return {0, 0};
            }
        };
        auto self = getPriority(this->getOperator());
        auto next_ = getPriority(next.getOperator());
        return self.first >= next_.second;
    }

    bool hasOperator() const override {
        return this->operator_ != '\0';
    }

    bool hasOperand() const override {
        return operand_valid;
    }

    ExpressionElement() {}
    ExpressionElement(char operator_): operator_(operator_) {}
    ExpressionElement(int operand): operand(operand), operand_valid(true) {}
};

// 输出接口
ostream& operator<<(ostream& out, const ExpressionElement& e) {
    if (e.hasOperator()) {
        out << e.getOperator();
        if (e.hasOperand()) {
            out << " " << e.getOperand();
        }
    } else {
        out << e.getOperand();
    }
    return out;
}


}
