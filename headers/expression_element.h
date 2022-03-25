#pragma once
#include <map>
#include "expression_framework.h"
#include "power.h"
#include "factorial.h"

// 这个文件实现了一个表达式元素的类
// 包括操作数和运算符两种类型

namespace clazy {

// 操作数
class Operand : public virtual clazy_framework::AbstractExpressionElement {
protected:
    int operand;
public:
    virtual bool isOperand() const { return true; }
    virtual int getOperand() const { return operand; }
    virtual void setOperand(int operand) { this->operand = operand; }
    Operand(int operand) : operand(operand) {}
};

// 下面实现运算符的类
// 在这个示例下包含的运算符包括
// 加、减、乘、除、求余、乘方、负号、阶乘
// 其中负号使用符号@表示，以避免与其他符号冲突
// 如果负号也使用-，那么这个框架就不能正常工作了，因为-号会被解释为减号
// 很容易构造出两个完全一样的后缀表达式，对应不同的中缀表达式，当负号和减号用同一个符号时
// 阶乘使用!表示，乘方使用^表示

static Power power;
static Factorial factorial;

// 运算符
class Operator : public virtual clazy_framework::AbstractExpressionElement {
protected:
    char operator_;
public:
    virtual bool isOperator() const { return true; }
    virtual char getOperator() const { return operator_; }
    virtual void setOperator(char operator_) { this->operator_ = operator_; }
    Operator(char operator_) : operator_(operator_) {}
    
    // 运算符左侧和右侧是否有操作数
    virtual pair<bool, bool> operandPosition() const {
        static const map<char, pair<bool, bool>> m = {
            {'(', {false, true}}, // 左括号右侧有操作数
            {')', {true, false}}, // 右括号左侧有操作数
            {'+', {true, true}},
            {'-', {true, true}},
            {'*', {true, true}},
            {'/', {true, true}},
            {'%', {true, true}},
            {'^', {true, true}},
            {'@', {false, true}},
            {'!', {true, false}}
        };
        return m.at(this->getOperator());
    }

    // 运算符的计算结果，对于操作数，返回自身。如果是单目运算符，空着的一边参数不做要求
    virtual int apply(int left, int right) const {
        switch (this->getOperator()) {
            case '+': return left + right;
            case '-': return left - right;
            case '*': return left * right;
            case '/': return left / right;
            case '%': return left % right;
            case '^': return power.apply(left, right);
            case '@': return - right;
            case '!': return factorial.apply(left);
            default: return 0; // not implemented
        }
    }

    // 运算符的优先级，如果本身是操作数是未定义行为
    virtual bool higherPriority(const AbstractExpressionElement& next) const {
        static const map<char, pair<int, int>> m = { // 这里分别是，在左侧的优先级，在右侧的优先级
            {'(', {0, 6}}, // 左括号出现在右边的时候优先级最高，而出现在左边的时候优先级最低
            {')', {0, 0}}, // 右括号永远不会进栈，优先级是最低的
            {'+', {1, 1}},
            {'-', {1, 1}},
            {'*', {2, 2}},
            {'/', {2, 2}},
            {'%', {2, 2}},
            {'^', {3, 3}},
            {'@', {4, 4}},
            {'!', {5, 5}}
        };
        auto self = m.at(this->getOperator());
        auto next_ = m.at(next.getOperator());
        return self.first >= next_.second; // 自己在左侧，下一个在右侧，如果运算符优先级相同，则先执行左边的
    }
};

// 混合的表达式元素，包括操作数和运算符
// 这个类表示的元素包括一个运算符和它右侧的操作数
class MixedElement : public Operator, public Operand {
public:
    MixedElement(clazy_framework::AbstractExpressionElement* e): Operator(e->getOperator()), Operand(e->getOperand()) {}
};

// 通用于三种表达式元素的输出接口
ostream& operator<<(ostream& out, const clazy_framework::AbstractExpressionElement& e) {
    if (!e.isOperator()) {
        return out << e.getOperand();
    } else {
        out << e.getOperator();
        if (e.isOperand()) {
            return out << "[" << e.getOperand() << "]";
        } else {
            return out;
        }
    }
}

}