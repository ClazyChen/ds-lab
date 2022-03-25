#pragma once

#include "framework.h"

// 这个文件讨论的是表达式
// 表达式是一个简单的语法树，用于计算表达式的值
// 表达式的语法树的结构是一个树，每个节点都是一个表达式
// 但另一方面，表达式是一个数和运算符组成的顺序表
// 从顺序表可以建立语法树，这个过程是通过栈（下推自动机）实现的
// 当然，也可以不建立语法树，直接从栈得到表达式的值

namespace clazy_framework {

// 表达式中的元素
class AbstractExpressionElement {
public:
    // 是否是运算符or操作数；获取运算符or操作数
    virtual bool isOperator() const { return false; }
    virtual bool isOperand() const { return false; }
    virtual char getOperator() const { return '\0'; }
    virtual int getOperand() const { return 0; }
    virtual void setOperator(char operator_) {}
    virtual void setOperand(int operand) {}

    // 左侧和右侧是否有操作数（对于操作数本身而言，它左右都不应该有操作数）
    virtual pair<bool, bool> operandPosition() const { return {false, false}; }

    // 运算符的计算结果，对于操作数，返回自身。如果是单目运算符，空着的一边参数不做要求
    virtual int apply(int left, int right) const { return getOperand(); }

    // 和下一个运算符相比的优先级比较结果，是否优先级高于下一个运算符（需要优先计算）
    virtual bool higherPriority(const AbstractExpressionElement& next) const { return false; }
};

// 抽象的表达式
// 中缀表达式、后缀表达式和表达式树等具体的表达式存储方式，都可以基于这个抽象的表达式进行
class AbstractExpression {
public:
    virtual int getResult() = 0;          // 计算表达式的值
    virtual void parse(string expr) = 0;  // 从输入的中缀表达式字符串转换
};

}